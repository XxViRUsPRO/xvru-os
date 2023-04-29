#include <types.h>
#include "disk.h"
#include "stdio.h"
#include "fat.h"
#include "memdefs.h"
#include "string.h"
#include "vesa.h"

#define PIXEL_OFFSET(x, y, pitch, bpp) (y * (pitch / (bpp / 8)) + x)
#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))

typedef void (*KernelEntry)(VbeInfoBlock *info);
u8 *kernel_load_buffer = (u8 *)MEMORY_LOAD_KERNEL;
u8 *kernel = (u8 *)MEMORY_KERNEL_ADDR;
const char *kernel_path = "/kernel.bin";

void __attribute__((cdecl)) start(u8 disk_id)
{
    clear_screen();
    disable_cursor();

    // Initialize disk
    disk_t disk;
    if (!disk_init(&disk, disk_id))
    {
        printf("Failed to initialize disk\n");
        return;
    }

    // Initialize FAT
    if (!fat_init(&disk))
    {
        printf("Failed to initialize FAT\n");
        return;
    }

    // Read kernel from disk into memory
    fat_file_t *file = fat_open(&disk, kernel_path);
    u32 read;
    u8 *kernel_buffer = kernel;
    while ((read = fat_read(&disk, file, MEMORY_LOAD_SIZE, kernel_load_buffer)) > 0)
    {
        memcpy(kernel_buffer, kernel_load_buffer, read);
        kernel_buffer += read;
    }
    fat_close(file);

    // Initialize VESA
    VbeInfoBlock *info = (VbeInfoBlock *)MEMORY_VBE_INFO_ADDR;
    if (!vbe_get_info(info))
    {
        printf("Failed to get VBE info\n");
        return;
    }

    // Find a suitable VESA mode
    VbeModeInfoBlock *mode_info = (VbeModeInfoBlock *)MEMORY_VBE_MODE_INFO_ADDR;
    u16 mode = 0xffff;
    u16 *modes = (u16 *)info->video_mode_ptr;
    u8 target_bpp = 32;
    u16 target_width = 800;
    u16 target_height = 600;
    for (u32 i = 0; modes[i] != 0xffff; i++)
    {
        if (vbe_get_mode_info(modes[i], mode_info))
        {
            if (mode_info->bpp == target_bpp && mode_info->width == target_width && mode_info->height == target_height)
            {
                mode = modes[i];
                break;
            }
        }
    }
    // If no suitable mode was found
    if (mode == 0xffff)
    {
        printf("Failed to find a suitable VESA mode\n");
        return;
    }

    // Set the VESA mode
    if (!vbe_set_mode(mode))
    {
        printf("Failed to set VESA mode\n");
        return;
    }

    // Draw a test pattern
    u32 *framebuffer = (u32 *)mode_info->framebuffer;
    u32 pitch = mode_info->pitch;
    u32 width = mode_info->width;
    u32 height = mode_info->height;
    u32 bpp = mode_info->bpp;
    i32 x, y;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            framebuffer[PIXEL_OFFSET(x, y, pitch, bpp)] = COLOR(x * 255 / width, y * 255 / height, 0);
        }
    }

    // Call the main function of the kernel
    KernelEntry kmain = (KernelEntry)kernel;
    kmain(mode_info);
}
