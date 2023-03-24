#include "memory.h"
#include <types.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for (; count != 0; count--)
        *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, u8 val, size_t count)
{
    char *temp = (char *)dest;
    for (; count != 0; count--)
        *temp++ = val;
    return dest;
}
