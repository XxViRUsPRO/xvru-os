#include "string.h"
#include "math.h"

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

void *memmove(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    if (sp < dp)
    {
        sp += count;
        dp += count;
        for (; count != 0; count--)
            *--dp = *--sp;
    }
    else
        for (; count != 0; count--)
            *dp++ = *sp++;
    return dest;
}

size_t strlen(const char *s)
{
    size_t len = 0;
    while (s[len])
        len++;
    return len;
}

int strcpy(char *dest, const char *src)
{
    size_t i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    return i;
}

char *strcat(char *dest, const char *src)
{
    size_t dest_len = strlen(dest);
    size_t i;
    for (i = 0; src[i] != '\0'; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';
    return dest;
}
