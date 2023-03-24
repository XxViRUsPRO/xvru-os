#include "string.h"

size_t strlen(const char *s)
{
    size_t len = 0;
    while (s[len])
        len++;
    return len;
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

char *itoa(int value, char *buffer, int base)
{
    char *p = buffer;
    char *p1, *p2;
    unsigned long ud = value;
    int divisor = 10;

    // If %d is specified and D is minus, put `-' in the head.
    if (base == 'd' && value < 0)
    {
        *p++ = '-';
        buffer++;
        ud = -value;
    }
    else if (base == 'x')
        divisor = 16;

    // Divide UD by DIVISOR until UD == 0.
    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    // Terminate BUF.
    *p = 0;

    // Reverse BUF.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return buffer;
}
