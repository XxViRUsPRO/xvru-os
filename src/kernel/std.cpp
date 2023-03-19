#include <stdint.h>
#include "std.h"

static uint32_t next = 1;

int rand(void)
{
    next = next * 1103515245 + 12345;
    return (uint32_t)(next / 65536) % 32768;
}

void srand(uint32_t seed)
{
    next = seed;
}

int strlen(const char *str)
{
    int len = 0;
    while (*str++)
        len++;
    return len;
}

char *itoa(int value, char *str, int base)
{
    char *rc;
    char *ptr;
    char *low;

    if (base < 2 || base > 36)
    {
        *str = '\0';
        return str;
    }

    rc = ptr = str;

    if (value < 0 && base == 10)
    {
        *ptr++ = '-';
    }

    low = ptr;

    do
    {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);

    *ptr-- = '\0';

    while (low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return rc;
}
