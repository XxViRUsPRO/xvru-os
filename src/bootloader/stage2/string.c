#include "string.h"

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
        for (sp += count, dp += count; count != 0; count--)
            *--dp = *--sp;
    }
    else
    {
        for (; count != 0; count--)
            *dp++ = *sp++;
    }
    return dest;
}
int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while (n-- > 0)
    {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++, p2++;
    }
    return 0;
}
void *memchr(const void *str, int c, size_t n)
{
    const unsigned char *p = str;
    while (n-- > 0)
    {
        if (*p == (unsigned char)c)
            return (void *)p;
        p++;
    }
    return NULL;
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
    char *temp = dest;
    while ((*dest++ = *src++) != '\0')
        ;
    return dest - temp - 1;
}
char *strcat(char *dest, const char *src)
{
    char *cp = dest;
    while (*cp)
        cp++;
    while ((*cp++ = *src++) != '\0')
        ;
    return dest;
}
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
char *strchr(const char *s, int c)
{
    for (; *s != (char)c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *)s;
}