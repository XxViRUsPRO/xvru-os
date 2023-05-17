#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"

typedef struct
{
    void *pageDirectory;
    void *memoryMap;
    u32 maxBlocks;
    u32 usedBlocks;
} MemoryManagerParams; // Memory Managers (VMM, PMM) Parameters

typedef struct
{
    void *kernelStart;
    void *kernelEnd;
    u32 kernelSize;
} KernelMemoryInfo;

typedef struct
{
    u64 base;
    u64 length;
    u32 type;
    u32 acpi;
} __attribute__((packed)) MemoryBlock;

typedef struct
{
    int count;
    MemoryBlock *blocks;
} MemoryPool;

typedef struct
{
    MemoryPool memoryPool;
    MemoryManagerParams mmParams;
    KernelMemoryInfo kernelMemoryInfo;
} KernelArgs;

#endif