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
} __attribute__((packed)) directory_entry_t;

typedef struct
{
    i32 handle;
    bool is_directory;
    u32 position;
    u32 size;
} fat_file_t;

enum FAT_Attributes
{
    FAT_ATTRIBUTE_READ_ONLY = 0x01,
    FAT_ATTRIBUTE_HIDDEN = 0x02,
    FAT_ATTRIBUTE_SYSTEM = 0x04,
    FAT_ATTRIBUTE_VOLUME_ID = 0x08,
    FAT_ATTRIBUTE_DIRECTORY = 0x10,
    FAT_ATTRIBUTE_ARCHIVE = 0x20,
    FAT_ATTRIBUTE_LFN = FAT_ATTRIBUTE_READ_ONLY | FAT_ATTRIBUTE_HIDDEN | FAT_ATTRIBUTE_SYSTEM | FAT_ATTRIBUTE_VOLUME_ID
};

bool fat_init(disk_t *disk);
fat_file_t *fat_open(disk_t *disk, const char *path);
u32 fat_read(disk_t *disk, fat_file_t *file, u32 byteCount, void *dataOut);
bool fat_read_entry(disk_t *disk, fat_file_t *file, directory_entry_t *dirEntry);
void fat_close(fat_file_t *file);

#endif