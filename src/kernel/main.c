#include <stdlib.h>
#include <stdio.h>
#include <hal/hal.h>
#include <types.h>
#include <system.h>
#include <math.h>
#include <x86.h>
#include <drivers/timer.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <display.h>

const u32 PMM_MEMORY_MAP_ADDRESS = 0x30000;

enum EMemoryBlockTypes
{
    MEMORY_BLOCK_FREE = 1,
    MEMORY_BLOCK_RESERVED = 2,
    MEMORY_BLOCK_ACPI_RECLAIMABLE = 3,
    MEMORY_BLOCK_ACPI_NVS = 4,
    MEMORY_BLOCK_BAD = 5,
};

void __attribute__((section(".entry"))) kmain(KernelArgs *kernelArgs)
{
    // Initialize the HAL
    HAL_init();
    timer_init();

    // Memory Map
    MemoryPool *memoryPool = &kernelArgs->memoryPool;
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
    fprintf(DEBUG_FD, "Total Memory: %d B (%f MB)\n", totalMemory, totalMemory / 1024 / 1024.0f);

    // Initialize the Physical Memory Manager
    pmm_init(PMM_MEMORY_MAP_ADDRESS, totalMemory);

    // Init memory regions with type 1
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        if (block->type == MEMORY_BLOCK_FREE)
        {
            pmm_init_region(block->base, block->length);
        }
    }

    // Reserve the memory from 0x0 to kenrel base which is 0x100000 + kernel size * 4
    pmm_deinit_region(0, largestBlock->base + (kernelArgs->kernelSize * 4));

    // Initialize the Virtual Memory Manager
    if (!vmm_init())
    {
        return;
    }

    // Map the framebuffer region
    const u32 fb_size = 800 * 600 * 4;
    u32 fb_size_pages = ceil((double)fb_size / 4096);
    fb_size_pages *= 2;
    for (u32 i = 0, fb_start = 0xfd000000; i < fb_size_pages; i++, fb_start += 4096)
    {
        vmm_map_page((void *)fb_start, (void *)fb_start);
    }

    // Deinit the framebuffer region
    u32 *fb = (u32 *)0xfd000000;
    pmm_deinit_region((u32)fb, fb_size_pages * 4096);

    // Unmap the kernel lower half
    // for (u32 i = 0x100000; i < 0x400000; i += 4096)
    // {
    //     vmm_unmap_page((void *)i);
    // }
    // vmm_unmap_page((void *)0x100000);

    // Flush the TLB
    __asm__ __volatile__("mov %cr3, %eax; mov %eax, %cr3;");

    display_init((void *)fb, 800, 600, 800 * 4, 32);
    display_clear(0x0000ff00);
    display_render();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
