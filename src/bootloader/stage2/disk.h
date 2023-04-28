#ifndef DISK_H
#define DISK_H
#include <types.h>

typedef struct
{
    u8 id;
    u16 cylinders;
    u16 sectors;
    u16 heads;
} disk_t;

bool disk_init(disk_t *disk, u8 id);
bool disk_read_sectors(disk_t *disk, u32 lba, u8 count, void *buffer);

#endif