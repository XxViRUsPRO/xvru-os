#include "stdio.h"
#include "string.h"
#include "x86.h"
#include "math.h"
#include <hal/hal.h>
#include <types.h>
#include <system.h>
#include <debug.h>
#include <drivers/timer.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <stdlib.h>

const u32 PMM_MEMORY_MAP_ADDRESS = 0x30000;

enum EMemoryBlockTypes
{
    MEMORY_BLOCK_FREE = 1,
    MEMORY_BLOCK_RESERVED = 2,
    MEMORY_BLOCK_ACPI_RECLAIMABLE = 3,
    MEMORY_BLOCK_ACPI_NVS = 4,
    MEMORY_BLOCK_BAD = 5,
};

char *memoryBlockTypeToString(u32 type)
{
    switch (type)
    {
    case MEMORY_BLOCK_FREE:
        return "Free";
    case MEMORY_BLOCK_RESERVED:
        return "Reserved";
    case MEMORY_BLOCK_ACPI_RECLAIMABLE:
        return "ACPI Reclaimable";
    case MEMORY_BLOCK_ACPI_NVS:
        return "ACPI NVS";
    case MEMORY_BLOCK_BAD:
        return "Bad";
    default:
        return "Unknown";
    }
}

void __attribute__((section(".entry"))) kmain(KernelArgs *kernelArgs)
{
    //! Initialize the HAL
    dbg_puts("Initializing the HAL\n");
    HAL_init();
    timer_init();

    //! Memory Map
    MemoryPool *memoryPool = &kernelArgs->memoryPool;
    MemoryBlock *blocks = memoryPool->blocks;
    u32 count = memoryPool->count;
    u32 totalMemory = 0;
    MemoryBlock *largestBlock = NULL;
    char str[256];
    sprintf(str, "Memory blocks: %d\n", count);
    dbg_puts(str);
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        totalMemory += block->length;
        sprintf(str, "Block %d (%s) - Base: 0x%x%x, Size: %f KB (%f MB)\n", i, memoryBlockTypeToString(block->type), (u32)(block->base >> 32), (u32)(block->base & 0xFFFFFFFF), (double)block->length / 1024, (double)block->length / 1024 / 1024);
        dbg_puts(str);
        if (block->type == MEMORY_BLOCK_FREE)
        {
            if (largestBlock == NULL || block->length > largestBlock->length)
                largestBlock = block;
        }
    }

    //! Initialize the Physical Memory Manager
    dbg_puts("Initializing the Physical Memory Manager\n");
    // pmm_init(PMM_MEMORY_MAP_ADDRESS, largestBlock->length);
    pmm_init(PMM_MEMORY_MAP_ADDRESS, totalMemory);

    // init memory regions with type 1
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        if (block->type == MEMORY_BLOCK_FREE)
        {
            pmm_init_region(block->base, block->length);
        }
    }

    // Set the largest block as free
    pmm_init_region(largestBlock->base, largestBlock->length);
    // Reserve the memory from 0x0 to kenrel base which is 0x100000 + kernel size * 4
    pmm_deinit_region(0, largestBlock->base + (kernelArgs->kernelSize * 4));

    // //! Initialize the Virtual Memory Manager
    dbg_puts("Initializing the Virtual Memory Manager\n");
    if (!vmm_init())
    {
        dbg_puts("Failed to initialize the Virtual Memory Manager\n");
        return;
    }

    // //! Map the framebuffer region
    const u32 fb_size = 800 * 600 * 4;
    u32 fb_size_pages = ceil((double)fb_size / 4096);
    fb_size_pages *= 2;
    for (u32 i = 0, fb_start = 0xfd000000; i < fb_size_pages; i++, fb_start += 4096)
    {
        vmm_map_page((void *)fb_start, (void *)fb_start);
    }

    // //! Deinit the framebuffer region
    u32 *fb = (u32 *)0xfd000000;
    pmm_deinit_region((u32)fb, fb_size);
    // fill screen with another test pattern to make sure that our idenity mapping is working correctly (draw rainbow pattern)
    for (u32 y = 0; y < 600; y++)
    {
        for (u32 x = 0; x < 800; x++)
        {
            u32 index = y * 800 + x;
            fb[index] = (x << 16) | (y << 8) | (x + y);
        }
    }

    // unmap the kernel lower half
    for (u32 i = 0x100000; i < 0x400000; i += 4096)
    {
        vmm_unmap_page((void *)i);
    }

    dbg_puts("Kernel initialized\n");

    void *ptr = malloc(100);
    sprintf(str, "Allocated block at 0x%x\n", ptr);
    dbg_puts(str);
    free(ptr);

    //! Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
