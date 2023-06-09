#ifndef FAT_H
#define FAT_H
#include <types.h>
#include "disk.h"

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
} __attribute__((packed)) DirectoryEntry;

typedef struct
{
    i32 handle;
    bool is_directory;
    u32 position;
    u32 size;
} FatFile;

enum EFatAttributes
{
    FAT_ATTRIBUTE_READ_ONLY = 0x01,
    FAT_ATTRIBUTE_HIDDEN = 0x02,
    FAT_ATTRIBUTE_SYSTEM = 0x04,
    FAT_ATTRIBUTE_VOLUME_ID = 0x08,
    FAT_ATTRIBUTE_DIRECTORY = 0x10,
    FAT_ATTRIBUTE_ARCHIVE = 0x20,
    FAT_ATTRIBUTE_LFN = FAT_ATTRIBUTE_READ_ONLY | FAT_ATTRIBUTE_HIDDEN | FAT_ATTRIBUTE_SYSTEM | FAT_ATTRIBUTE_VOLUME_ID
};

bool fat_init(Disk *disk);
FatFile *fat_open(Disk *disk, const char *path);
u32 fat_read(Disk *disk, FatFile *file, u32 byteCount, void *dataOut);
bool fat_read_entry(Disk *disk, FatFile *file, DirectoryEntry *dirEntry);
void fat_close(FatFile *file);

#endif