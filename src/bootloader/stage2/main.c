#include <types.h>
#include "disk.h"
#include "stdio.h"
#include "fat.h"
#include "memdefs.h"
#include "memdetect.h"
#include <string.h>
#include "vesa.h"
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <math.h>

#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))

KernelArgs g_KernelArgs;
u8 *kernel_load_buffer = (u8 *)MEMORY_LOAD_KERNEL;
u8 *kernel = (u8 *)MEMORY_KERNEL_ADDR;

enum EMemoryBlockTypes
{
    MEMORY_BLOCK_FREE = 1,
    MEMORY_BLOCK_RESERVED = 2,
    MEMORY_BLOCK_ACPI_RECLAIMABLE = 3,
    MEMORY_BLOCK_ACPI_NVS = 4,
    MEMORY_BLOCK_BAD = 5,
};

void __attribute__((cdecl)) start(u8 disk_id)
{
    // Initialize disk
    Disk disk;
    if (!disk_init(&disk, disk_id))
    {
        console_log("Failed to initialize disk\n");
        return;
    }

    // Initialize FAT
    if (!fat_init(&disk))
    {
        console_log("Failed to initialize FAT\n");
        return;
    }

    // Initialize VESA
    VbeInfoBlock *info = (VbeInfoBlock *)MEMORY_VBE_INFO_ADDR;
    if (!vbe_get_info(info))
    {
        console_log("Failed to get VBE info\n");
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
        console_log("Failed to find a suitable VESA mode\n");
        return;
    }

    // Set the VESA mode
    if (!vbe_set_mode(mode))
    {
        console_log("Failed to set VESA mode\n");
        return;
    }

    // Initialize memory
    mem_get_map(&g_KernelArgs.memoryPool);

    // Memory Map
    MemoryPool *memoryPool = &g_KernelArgs.memoryPool;
    MemoryBlock *blocks = memoryPool->blocks;
    u32 count = memoryPool->count;
    u32 totalMemory = 0;
    MemoryBlock *largestBlock = NULL;
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        totalMemory += block->length;
        if (block->type == MEMORY_BLOCK_FREE)
        {
            if (largestBlock == NULL || block->length > largestBlock->length)
                largestBlock = block;
        }
    }

    // Initialize the Physical Memory Manager
    pmm_init((u32)MEMORY_PMM_ADDR, totalMemory);

    // Init memory regions with type 1
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        if (block->type == MEMORY_BLOCK_FREE)
        {
            pmm_init_region(block->base, block->length);
        }
    }

    // Read kernel from disk into memory
    FatFile *file = fat_open(&disk, "/kernel.bin");
    g_KernelArgs.kernelMemoryInfo.kernelStart = (void *)MEMORY_KERNEL_ADDR;
    g_KernelArgs.kernelMemoryInfo.kernelSize = file->size;
    g_KernelArgs.kernelMemoryInfo.kernelEnd = (void *)(MEMORY_KERNEL_ADDR + g_KernelArgs.kernelMemoryInfo.kernelSize * 4);

    // Deinit memory region from 0x0 to kernelEnd
    pmm_deinit_region(0x0, (u32)g_KernelArgs.kernelMemoryInfo.kernelEnd);

    u32 read;
    u8 *kernel_buffer = kernel;
    while ((read = fat_read(&disk, file, MEMORY_LOAD_SIZE, kernel_load_buffer)) > 0)
    {
        memcpy(kernel_buffer, kernel_load_buffer, read);
        kernel_buffer += read;
    }
    fat_close(file);

    // Initialize the Virtual Memory Manager
    if (!vmm_init())
    {
        return;
    }

    // Unmap the kernel lower half
    for (u32 i = 0x100000; i < 0x400000; i += 4096)
    {
        vmm_unmap_page((void *)i);
    }

    // Flush the TLB
    __asm__ __volatile__("mov %cr3, %eax; mov %eax, %cr3;");

    // Pass Page Directory to the kernel
    g_KernelArgs.mmParams.memoryMap = (void *)pmm_get_memory_map();
    g_KernelArgs.mmParams.maxBlocks = pmm_get_max_blocks();
    g_KernelArgs.mmParams.usedBlocks = pmm_get_used_blocks();
    g_KernelArgs.mmParams.pageDirectory = (void *)vmm_get_pdirectory();

    // Call the main function of the kernel
    ((void (*)(KernelArgs *))0xC0000000)(&g_KernelArgs);
}
