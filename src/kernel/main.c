#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>
#include <system.h>
#include <debug.h>
#include <drivers/timer.h>

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
        memset(str, 0, 256);
        MemoryBlock *block = &blocks[i];
        sprintf(str, "Memory block %d: %f KB\n", i, block->length / 1024.0f);
        dbg_puts(str);

        if (largestBlock == NULL || block->length > largestBlock->length)
            largestBlock = block;
    }
    if (largestBlock != NULL)
    {
        memset(str, 0, 256);
        sprintf(str, "Largest memory block: %f KB at 0x%x\n", largestBlock->length / 1024.0f, largestBlock->base);
        dbg_puts(str);
    }
    else
    {
        dbg_puts("No memory blocks found\n");
        return;
    }

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
