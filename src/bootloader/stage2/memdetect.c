#include "memdetect.h"
#include "x86.h"
#include <types.h>
#include <string.h>

#define MAX_MEMORY_ENTRIES 256

MemoryBlock g_MemoryBlocks[MAX_MEMORY_ENTRIES];
u32 g_MemoryBlockCount = 0;

void mem_get_map(MemoryPool *memoryPool)
{
    MemoryBlock entry;
    u32 cont_id = 0;
    i32 result;

    g_MemoryBlockCount = 0;
    result = x86_mem_map_(&entry, &cont_id);

    while (result > 0 && cont_id != 0)
    {
        memcpy(&g_MemoryBlocks[g_MemoryBlockCount], &entry, sizeof(MemoryBlock));
        g_MemoryBlockCount++;
        result = x86_mem_map_(&entry, &cont_id);
    }

    memoryPool->blocks = g_MemoryBlocks;
    memoryPool->count = g_MemoryBlockCount;
}