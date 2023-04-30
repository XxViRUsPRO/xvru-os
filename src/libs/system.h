#ifndef SYSTEM_H
#define SYSTEM_H
#include "types.h"

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
} KernelArgs;

#endif