#ifndef KHEAP_H
#define KHEAP_H
#include <types.h>
#include <string.h>

u32 kmalloc(u32 size);
u32 kmalloc_page();

#endif