#include <types.h>
#include "disk.h"
#include "stdio.h"
#include "fat.h"
#include "memdefs.h"
#include "string.h"

typedef void (*KernelEntry)(void);
u8 *kernel_load_buffer = (u8 *)MEMORY_LOAD_KERNEL;
u8 *kernel = (u8 *)MEMORY_KERNEL_ADDR;

const char *welcome_message = "Welcome to the stage 2 of the bootloader!\n";
const char *kernel_path = "/kernel.bin";

void __attribute__((cdecl)) start(u8 disk_id)
{
    clear_screen();
    disable_cursor();

    // center the welcome message
    u32 welcome_message_length = strlen(welcome_message);
    u32 welcome_message_offset = (MAX_COLS - welcome_message_length) / 2;
    set_cursor(welcome_message_offset, (MAX_ROWS / 2) - 1);
    printf(welcome_message);

    disk_t disk;
    if (!disk_init(&disk, disk_id))
    {
        printf("Failed to initialize disk\n");
        return;
    }

    if (!fat_init(&disk))
    {
        printf("Failed to initialize FAT\n");
        return;
    }

    fat_file_t *file = fat_open(&disk, kernel_path);
    u32 read;
    u8 *kernel_buffer = kernel;
    while ((read = fat_read(&disk, file, MEMORY_LOAD_SIZE, kernel_load_buffer)) > 0)
    {
        memcpy(kernel_buffer, kernel_load_buffer, read);
        kernel_buffer += read;
    }
    fat_close(file);

    // Call the main function of the kernel
    KernelEntry kmain = (KernelEntry)kernel;
    kmain();
}
