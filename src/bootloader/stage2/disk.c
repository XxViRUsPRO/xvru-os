#include "disk.h"
#include "x86.h"
#include "stdio.h"

bool disk_init(disk_t *disk, u8 id)
{
    u8 drive_type;
    u16 cylinders, sectors, heads;
    if (!x86_disk_get_params_(disk->id, &drive_type, &cylinders, &sectors, &heads))
        return false;
    disk->id = id;
    disk->cylinders = cylinders;
    disk->sectors = sectors;
    disk->heads = heads;
    return true;
}

void disk_lba2chs(disk_t *disk, u32 lba, u16 *cylinder, u16 *head, u16 *sector)
{
    // sector = (LBA % sectors per track + 1)
    *sector = (lba % disk->sectors) + 1;

    // cylinder = (LBA / sectors per track) / heads
    *cylinder = (lba / disk->sectors) / disk->heads;

    // head = (LBA / sectors per track) % heads
    *head = (lba / disk->sectors) % disk->heads;
}

bool disk_read_sectors(disk_t *disk, u32 lba, u8 count, void *buffer)
{
    u16 cylinder, head, sector;
    disk_lba2chs(disk, lba, &cylinder, &head, &sector);
    // Read the sectors 3 times in case of failure (retry)
    for (u32 i = 0; i < 3; i++)
    {
        if (x86_disk_read_(disk->id, cylinder, sector, head, count, buffer))
        {
            return true;
        }
        x86_disk_reset_(disk->id);
    }
    return false;
}