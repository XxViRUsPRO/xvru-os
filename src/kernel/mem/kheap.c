#include "kheap.h"

extern u32 KERNEL_END;
u32 mem_addr = (u32)&KERNEL_END;

u32 kmalloc(u32 size)
{
    u32 mem = mem_addr;
    mem_addr += size;
    return mem;
}

u32 kmalloc_page()
{
    if (mem_addr & 0xFFFFF000)
    {
        mem_addr &= 0xFFFFF000;
        mem_addr += 0x1000;
    }
    return kmalloc(0x1000);
}