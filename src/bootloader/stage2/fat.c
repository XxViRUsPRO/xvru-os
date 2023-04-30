#include "fat.h"
#include "memdefs.h"
#include <string.h>
#include "utils.h"
#include "stdio.h"
#include <ctype.h>

#define SECTOR_SIZE 512
#define MAX_PATH_SIZE 256
#define MAX_FILE_HANDLES 10
#define ROOT_DIRECTORY_HANDLE -1

typedef struct
{
    // BIOS Parameter Block
    u8 jmp[3];
    u8 oem[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sectors;
    u8 number_of_fats;
    u16 root_dir_entries;
    u16 total_sectors;
    u8 media_descriptor;
    u16 sectors_per_fat;
    u16 sectors_per_track;
    u16 number_of_heads;
    u32 hidden_sectors;
    u32 large_total_sectors;
    // Extended Boot Record
    u8 drive_number;
    u8 reserved;
    u8 boot_signature;
    u32 volume_id;
    u8 volume_label[11];
    u8 system_id[8];
} __attribute__((packed)) FatHeader;

typedef struct
{
    u8 buffer[SECTOR_SIZE];
    FatFile public;
    bool opened;
    u32 first_cluster;
    u32 current_cluster;
    u32 current_selector;

} FatFileData;

typedef struct
{
    union
    {
        FatHeader BootSector;
        u8 BootSectorBytes[SECTOR_SIZE];
    } BS;

    FatFileData RootDirectory;

    FatFileData OpenedFiles[MAX_FILE_HANDLES];

} FatData;

static FatData *g_Data;
static u8 *g_Fat = NULL;
static u32 g_DataSectionLba;

bool fat_read_header(Disk *disk)
{
    return disk_read_sectors(disk, 0, 1, g_Data->BS.BootSectorBytes);
}

bool fat_read_fat(Disk *disk)
{
    return disk_read_sectors(disk, g_Data->BS.BootSector.reserved_sectors, g_Data->BS.BootSector.sectors_per_fat, g_Fat);
}

bool fat_init(Disk *disk)
{
    g_Data = (FatData *)MEMORY_FAT_ADDR;

    // read boot sector
    if (!fat_read_header(disk))
    {
        printf("FAT: read boot sector failed\r\n");
        return false;
    }

    // read FAT
    g_Fat = (u8 *)g_Data + sizeof(FatData);
    u32 fatSize = g_Data->BS.BootSector.bytes_per_sector * g_Data->BS.BootSector.sectors_per_fat;
    if (sizeof(FatData) + fatSize >= MEMORY_FAT_SIZE)
    {
        printf("FAT: not enough memory to read FAT! Required %d, only have %d\r\n", sizeof(FatData) + fatSize, MEMORY_FAT_SIZE);
        return false;
    }

    if (!fat_read_fat(disk))
    {
        printf("FAT: read FAT failed\r\n");
        return false;
    }

    // open root directory file
    u32 rootDirLba = g_Data->BS.BootSector.reserved_sectors + g_Data->BS.BootSector.sectors_per_fat * g_Data->BS.BootSector.number_of_fats;
    u32 rootDirSize = sizeof(DirectoryEntry) * g_Data->BS.BootSector.root_dir_entries;

    g_Data->RootDirectory.public.handle = ROOT_DIRECTORY_HANDLE;
    g_Data->RootDirectory.public.is_directory = true;
    g_Data->RootDirectory.public.position = 0;
    g_Data->RootDirectory.public.size = sizeof(DirectoryEntry) * g_Data->BS.BootSector.root_dir_entries;
    g_Data->RootDirectory.opened = true;
    g_Data->RootDirectory.first_cluster = rootDirLba;
    g_Data->RootDirectory.current_cluster = rootDirLba;
    g_Data->RootDirectory.current_selector = 0;

    if (!disk_read_sectors(disk, rootDirLba, 1, g_Data->RootDirectory.buffer))
    {
        printf("FAT: read root directory failed\r\n");
        return false;
    }

    // calculate data section
    u32 rootDirSectors = (rootDirSize + g_Data->BS.BootSector.bytes_per_sector - 1) / g_Data->BS.BootSector.bytes_per_sector;
    g_DataSectionLba = rootDirLba + rootDirSectors;

    // reset opened files
    for (int i = 0; i < MAX_FILE_HANDLES; i++)
        g_Data->OpenedFiles[i].opened = false;

    return true;
}

u32 fat_cluster2lba(u32 cluster)
{
    return g_DataSectionLba + (cluster - 2) * g_Data->BS.BootSector.sectors_per_cluster;
}

FatFile *fat_open_entry(Disk *disk, DirectoryEntry *entry)
{
    // find empty handle
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++)
    {
        if (!g_Data->OpenedFiles[i].opened)
            handle = i;
    }

    // out of handles
    if (handle < 0)
    {
        printf("FAT: out of file handles\r\n");
        return false;
    }

    // setup vars
    FatFileData *fd = &g_Data->OpenedFiles[handle];
    fd->public.handle = handle;
    fd->public.is_directory = (entry->attributes & FAT_ATTRIBUTE_DIRECTORY) != 0;
    fd->public.position = 0;
    fd->public.size = entry->file_size;
    fd->first_cluster = entry->first_cluster_low + ((u32)entry->first_cluster_high << 16);
    fd->current_cluster = fd->first_cluster;
    fd->current_selector = 0;

    if (!disk_read_sectors(disk, fat_cluster2lba(fd->current_cluster), 1, fd->buffer))
    {
        printf("FAT: open entry failed - read error cluster=%d lba=%d\n", fd->current_cluster, fat_cluster2lba(fd->current_cluster));
        for (int i = 0; i < 11; i++)
            printf("%c", entry->name[i]);
        printf("\n");
        return false;
    }

    fd->opened = true;
    return &fd->public;
}

u32 fat_next_cluster(u32 current_cluster)
{
    u32 fatIndex = current_cluster * 3 / 2;

    if (current_cluster % 2 == 0)
        return (*(u16 *)(g_Fat + fatIndex)) & 0x0FFF;
    else
        return (*(u16 *)(g_Fat + fatIndex)) >> 4;
}

u32 fat_read(Disk *disk, FatFile *file, u32 byteCount, void *dataOut)
{
    // get file data
    FatFileData *fd = (file->handle == ROOT_DIRECTORY_HANDLE)
                          ? &g_Data->RootDirectory
                          : &g_Data->OpenedFiles[file->handle];

    u8 *u8DataOut = (u8 *)dataOut;

    // don't read past the end of the file
    if (!fd->public.is_directory || (fd->public.is_directory && fd->public.size != 0))
        byteCount = min(byteCount, fd->public.size - fd->public.position);

    while (byteCount > 0)
    {
        u32 leftInbuffer = SECTOR_SIZE - (fd->public.position % SECTOR_SIZE);
        u32 take = min(byteCount, leftInbuffer);

        memcpy(u8DataOut, fd->buffer + fd->public.position % SECTOR_SIZE, take);
        u8DataOut += take;
        fd->public.position += take;
        byteCount -= take;

        // See if we need to read more data
        if (leftInbuffer == take)
        {
            // Special handling for root directory
            if (fd->public.handle == ROOT_DIRECTORY_HANDLE)
            {
                ++fd->current_cluster;

                // read next sector
                if (!disk_read_sectors(disk, fd->current_cluster, 1, fd->buffer))
                {
                    printf("FAT: read error!\r\n");
                    break;
                }
            }
            else
            {
                // calculate next cluster & sector to read
                if (++fd->current_selector >= g_Data->BS.BootSector.sectors_per_cluster)
                {
                    fd->current_selector = 0;
                    fd->current_cluster = fat_next_cluster(fd->current_cluster);
                }

                if (fd->current_cluster >= 0xFF8)
                {
                    // Mark end of file
                    fd->public.size = fd->public.position;
                    break;
                }

                // read next sector
                if (!disk_read_sectors(disk, fat_cluster2lba(fd->current_cluster) + fd->current_selector, 1, fd->buffer))
                {
                    printf("FAT: read error!\r\n");
                    break;
                }
            }
        }
    }

    return u8DataOut - (u8 *)dataOut;
}

bool fat_read_entry(Disk *disk, FatFile *file, DirectoryEntry *dirEntry)
{
    return fat_read(disk, file, sizeof(DirectoryEntry), dirEntry) == sizeof(DirectoryEntry);
}

void fat_close(FatFile *file)
{
    if (file->handle == ROOT_DIRECTORY_HANDLE)
    {
        file->position = 0;
        g_Data->RootDirectory.current_cluster = g_Data->RootDirectory.first_cluster;
    }
    else
    {
        g_Data->OpenedFiles[file->handle].opened = false;
    }
}

bool fat_find_file(Disk *disk, FatFile *file, const char *name, DirectoryEntry *entryOut)
{
    char fatName[12];
    DirectoryEntry entry;

    // convert from name to fat name
    memset(fatName, ' ', sizeof(fatName));
    fatName[11] = '\0';

    const char *ext = strchr(name, '.');
    if (ext == NULL)
        ext = name + 11;

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++)
        fatName[i] = toupper(name[i]);

    if (ext != name + 11)
    {
        for (int i = 0; i < 3 && ext[i + 1]; i++)
            fatName[i + 8] = toupper(ext[i + 1]);
    }

    while (fat_read_entry(disk, file, &entry))
    {
        if (memcmp(fatName, entry.name, 11) == 0)
        {
            // *entryOut = entry;
            memcpy(entryOut, &entry, sizeof(DirectoryEntry));
            return true;
        }
    }

    return false;
}

FatFile *fat_open(Disk *disk, const char *path)
{
    char name[MAX_PATH_SIZE];

    // ignore leading slash
    if (path[0] == '/')
        path++;

    FatFile *current = &g_Data->RootDirectory.public;

    while (*path)
    {
        // extract next file name from path
        bool isLast = false;
        const char *delim = strchr(path, '/');
        if (delim != NULL)
        {
            memcpy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        }
        else
        {
            unsigned len = strlen(path);
            memcpy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }

        // find directory entry in current directory
        DirectoryEntry entry;
        if (fat_find_file(disk, current, name, &entry))
        {
            fat_close(current);

            // check if directory
            if (!isLast && entry.attributes & FAT_ATTRIBUTE_DIRECTORY == 0)
            {
                printf("FAT: %s not a directory\r\n", name);
                return NULL;
            }

            // open new directory entry
            current = fat_open_entry(disk, &entry);
        }
        else
        {
            fat_close(current);

            printf("FAT: %s not found\r\n", name);
            return NULL;
        }
    }

    return current;
}