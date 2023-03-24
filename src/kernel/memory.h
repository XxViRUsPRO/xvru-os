#ifndef _MEMORY_H_
#define _MEMORY_H_
#include <types.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, u8 val, size_t count);

#endif