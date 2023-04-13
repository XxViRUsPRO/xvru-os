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

int itoa(int value, char *buffer, int base)
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

    return p - buffer;
}

int ftoa(float value, char *buffer, int afterpoint)
{
    // Extract integer part
    int ipart = (int)value;

    // Extract floating part
    float fpart = value - (float)ipart;

    // convert integer part to string
    int i = itoa(ipart, buffer, 10);

    // check for display option after point
    if (afterpoint != 0)
    {
        buffer[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        i += itoa((int)fpart, buffer + i + 1, 10) + (afterpoint - 1);
    }
    return i;
}
