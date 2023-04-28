#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char bool;
#define true 1
#define false 0

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
} __attribute__((packed)) fat_header_t;

typedef struct
{
    u8 name[11];
    u8 attributes;
    u8 reserved;
    u8 creation_time_tenths;
    u16 creation_time;
    u16 creation_date;
    u16 last_access_date;
    u16 first_cluster_high;
    u16 last_modification_time;
    u16 last_modification_date;
    u16 first_cluster_low;
    u32 file_size;
} __attribute__((packed)) directory_entry_t;

// Global variables
fat_header_t g_fat_header;
u8 *g_fat = NULL;
directory_entry_t *g_root_dir = NULL;
u32 g_root_dir_end = 0;

bool read_fat_header(FILE *disk)
{
    fseek(disk, 0, SEEK_SET);
    return fread(&g_fat_header, sizeof(fat_header_t), 1, disk) > 0;
}
bool read_sectors(FILE *disk, u32 lba, u32 count, void *buffer)
{
    bool result = true;
    result = result && (fseek(disk, lba * g_fat_header.bytes_per_sector, SEEK_SET) == 0);
    result = result && (fread(buffer, g_fat_header.bytes_per_sector, count, disk) > 0);
    return result;
}
bool read_fat(FILE *disk)
{
    if (g_fat)
        free(g_fat);
    g_fat = malloc(g_fat_header.sectors_per_fat * g_fat_header.bytes_per_sector);
    return read_sectors(disk, g_fat_header.reserved_sectors, g_fat_header.sectors_per_fat, g_fat);
}
bool read_root_dir(FILE *disk)
{
    if (g_root_dir)
        free(g_root_dir);
    u32 lba = g_fat_header.reserved_sectors + g_fat_header.number_of_fats * g_fat_header.sectors_per_fat;
    u32 size = g_fat_header.root_dir_entries * sizeof(directory_entry_t);
    u32 sectors = size / g_fat_header.bytes_per_sector;
    if (size % g_fat_header.bytes_per_sector)
        sectors++;
    g_root_dir_end = lba + sectors;
    g_root_dir = (directory_entry_t *)malloc(sectors * g_fat_header.bytes_per_sector);
    return read_sectors(disk, lba, sectors, g_root_dir);
}
directory_entry_t *find_file(const char *filename)
{
    for (u32 i = 0; i < g_fat_header.root_dir_entries; i++)
    {
        if (memcmp(g_root_dir[i].name, filename, 11) == 0)
            return &g_root_dir[i];
    }
    return NULL;
}
int ceil(double x)
{
    int i = (int)x;
    if (x > i)
        return i + 1;
    return i;
}
void print_file_metadata(directory_entry_t *file)
{
    printf("Name: %s (%x)\n", file->name, file->attributes);
    printf("Size: %d bytes (%d clusters) (first cluster: %d)\n", file->file_size, ceil((double)file->file_size / (g_fat_header.sectors_per_cluster * g_fat_header.bytes_per_sector)), file->first_cluster_low);
    printf("Creation time: %d:%d:%d\n", file->creation_time >> 11, (file->creation_time >> 5) & 0x3F, (file->creation_time & 0x1F) * 2);
    printf("Creation date: %d/%d/%d\n", (file->creation_date >> 5) & 0xF, file->creation_date & 0x1F, (file->creation_date >> 9) + 1980);
    printf("Last modification time: %d:%d:%d\n", file->last_modification_time >> 11, (file->last_modification_time >> 5) & 0x3F, (file->last_modification_time & 0x1F) * 2);
    printf("Last modification date: %d/%d/%d\n", (file->last_modification_date >> 5) & 0xF, file->last_modification_date & 0x1F, (file->last_modification_date >> 9) + 1980);
    printf("Last access date: %d/%d/%d\n", (file->last_access_date >> 5) & 0xF, file->last_access_date & 0x1F, (file->last_access_date >> 9) + 1980);
}
bool read_file(FILE *disk, directory_entry_t *file, void *buffer)
{
    u16 current_cluster = file->first_cluster_low;
    bool result = false;

    do
    {
        u32 lba = g_root_dir_end + (current_cluster - 2) * g_fat_header.sectors_per_cluster;
        result = read_sectors(disk, lba, g_fat_header.sectors_per_cluster, buffer);
        buffer += g_fat_header.sectors_per_cluster * g_fat_header.bytes_per_sector;

        u32 next_cluster = current_cluster * 3 / 2;
        if (current_cluster % 2)
            current_cluster = (*(u16 *)&g_fat + next_cluster) >> 4;
        else
            current_cluster = (*(u16 *)&g_fat + next_cluster) & 0x0FFF;
    } while (!result && current_cluster < 0xFF8);

    return result;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Syntax: %s <disk image> <file>\n", argv[0]);
        return 1;
    }

    FILE *disk = fopen(argv[1], "rb");
    if (!disk)
    {
        fprintf(stderr, "Error: could not open disk image %s\n", argv[1]);
        return 1;
    }
    if (!read_fat_header(disk))
    {
        fprintf(stderr, "Error: could not read FAT header\n");
        return 1;
    }
    if (!read_fat(disk))
    {
        fprintf(stderr, "Error: could not read FAT\n");
        free(g_fat);
        return 1;
    }
    if (!read_root_dir(disk))
    {
        fprintf(stderr, "Error: could not read root directory\n");
        free(g_fat);
        free(g_root_dir);
        return 1;
    }

    directory_entry_t *file = find_file(argv[2]);
    if (!file)
    {
        fprintf(stderr, "Error: file %s not found\n", argv[2]);
        free(g_fat);
        free(g_root_dir);
        return 1;
    }
    print_file_metadata(file);

    // void *buffer = malloc(file->file_size + g_fat_header.bytes_per_sector);
    // if (!read_file(disk, file, buffer))
    // {
    //     fprintf(stderr, "Error: could not read file %s\n", argv[2]);
    //     free(g_fat);
    //     free(g_root_dir);
    //     free(buffer);
    //     return 1;
    // }
    // printf("File contents:\n");
    // for (u32 i = 0; i < file->file_size; i++)
    // {
    //     printf("%02X ", ((u8 *)buffer)[i]);
    //     if ((i + 1) % 16 == 0)
    //         printf("\n");
    // }
    // printf("\n");

    free(g_fat);
    free(g_root_dir);
    // free(buffer);
    return 0;
}