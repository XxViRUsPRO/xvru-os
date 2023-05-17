#include <string.h>
#include <stdio.h>
#include <math.h>
#include <hal/vfs.h>
#include <ctype.h>

void fputc(fd_t fd, char c)
{
    vfs_write(fd, &c, 1);
}

void fputs(fd_t fd, const char *str)
{
    vfs_write(fd, str, strlen(str));
}

void vfprintf(fd_t fd, const char *fmt, va_list arg)
{
    char *p;
    char *sval;
    int ival;
    float fval;
    char buf[32];

    for (p = (char *)fmt; *p; p++)
    {
        if (*p != '%')
        {
            fputc(fd, *p);
            continue;
        }
        switch (*++p)
        {
        case 'c':
            ival = va_arg(arg, int);
            fputc(fd, ival);
            break;
        case 's':
            for (sval = va_arg(arg, char *); *sval; sval++)
                fputc(fd, *sval);
            break;
        case 'd':
            ival = va_arg(arg, int);
            itoa(ival, buf, 10);
            fputs(fd, buf);
            break;
        case 'x':
            ival = va_arg(arg, int);
            itoa(ival, buf, 16);
            fputs(fd, buf);
            break;
        case 'f':
            fval = va_arg(arg, double);
            // TODO: Add precision handling
            int precision = 6;
            ftoa(fval, buf, precision);
            fputs(fd, buf);
            break;
        default:
            fputc(fd, *p);
            break;
        }
    }
}

void fprintf(fd_t fd, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vfprintf(fd, fmt, arg);
    va_end(arg);
}

void putc(char c)
{
    fputc(STDOUT_FD, c);
}

void puts(const char *str)
{
    fputs(STDOUT_FD, str);
}

void printf(const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    vfprintf(STDOUT_FD, fmt, arg);
    va_end(arg);
}

int itoa(int value, char *buffer, int base)
{
    char *p = buffer;
    char *p1, *p2;
    unsigned long ud = value;
    int divisor = 10;

    // If base is 10 and value is negative, put a '-' in the buffer.
    if (base == 10 && value < 0)
    {
        *p++ = '-';
        buffer++;
        ud = -value;
    }
    else if (base == 16)
    {
        divisor = 16;
    }

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

int atoi(const char *str)
{
    int res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    // return result.
    return res;
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
