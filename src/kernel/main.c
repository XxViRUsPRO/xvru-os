#include "stdio.h"
#include "string.h"
#include "x86.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>
#include <system.h>
#include <debug.h>
#include <drivers/timer.h>
#include <mem/pmm.h>
#include <mem/vmm.h>

const u32 PMM_MEMORY_MAP_ADDRESS = 0x50000;

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
    HAL_init();
    timer_init();

    // Memory Map
    MemoryPool *memoryPool = &kernelArgs->memoryPool;
    MemoryBlock *blocks = memoryPool->blocks;
    u32 count = memoryPool->count;
    MemoryBlock *largestBlock = NULL;
    char str[256];
    sprintf(str, "Memory blocks: %d\n", count);
    dbg_puts(str);
    for (u32 i = 0; i < count; i++)
    {
        MemoryBlock *block = &blocks[i];
        sprintf(str, "Block %d (%s) - Base: 0x%x%x, Size: %f KB (%f MB)\n", i, memoryBlockTypeToString(block->type), (u32)(block->base >> 32), (u32)(block->base & 0xFFFFFFFF), (double)block->length / 1024, (double)block->length / 1024 / 1024);
        dbg_puts(str);
        if (block->type == MEMORY_BLOCK_FREE)
        {
            if (largestBlock == NULL || block->length > largestBlock->length)
                largestBlock = block;
        }
    }

    // Initialize the Physical Memory Manager
    pmm_init(PMM_MEMORY_MAP_ADDRESS, largestBlock->length);
    // Set the largest block as free
    pmm_init_region(largestBlock->base, largestBlock->length);
    // Reserve the memory from 0x0 to kernel end boundary
    pmm_deinit_region(0, largestBlock->base);

    // Initialize the Virtual Memory Manager
    if (!vmm_init())
    {
        dbg_puts("Failed to initialize the Virtual Memory Manager\n");
        return;
    }

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
