#include <stdint.h>
#include "memory.h"

void memset(void *dest, uint8_t value, uint32_t size)
{
    uint8_t *d = (uint8_t *)dest;
    for (uint32_t i = 0; i < size; i++)
    {
        d[i] = value;
    }
}

void memcpy(void *dest, void *src, uint32_t size)
{
    uint8_t *d = (uint8_t *)dest;
    uint8_t *s = (uint8_t *)src;
    for (uint32_t i = 0; i < size; i++)
    {
        d[i] = s[i];
    }
}
