#include "stdio.h"
#include "string.h"
#include "x86.h"
#include "math.h"
#include <stdarg.h>

void putchar(int x, int y, char c, char color)
{
    vidmem[2 * (y * MAX_COLS + x)] = c;
    vidmem[2 * (y * MAX_COLS + x) + 1] = color;
}

void putcolor(int x, int y, char color)
{
    vidmem[2 * (y * MAX_COLS + x) + 1] = color;
}

char getchar(int x, int y)
{
    return vidmem[2 * (y * MAX_COLS + x)];
}

u8 getcolor(int x, int y)
{
    return vidmem[2 * (y * MAX_COLS + x) + 1];
}

void set_cursor(int x, int y)
{
    // Set the cursor position
    cursor_x = x;
    cursor_y = y;
    unsigned short pos = cursor_y * MAX_COLS + cursor_x;

    // Send the high byte
    outb(0x3D4, 14);
    outb(0x3D5, (unsigned char)(pos >> 8));

    // Send the low byte
    outb(0x3D4, 15);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
}

void clear_screen()
{
    // Clear the screen
    for (int i = 0; i < MAX_COLS * MAX_ROWS; i++)
    {
        // Write a space to the screen
        putchar(i % MAX_COLS, i / MAX_COLS, ' ', DEFAULT_COLOR);
    }

    // Reset the cursor position
    cursor_x = 0;
    cursor_y = 0;
    set_cursor(cursor_x, cursor_y);
}

void enable_cursor(u8 cursor_start, u8 cursor_end)
{
    // Enable the cursor
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor(void)
{
    // Disable the cursor
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void scroll()
{
    // Scroll the screen
    for (int i = 0; i < MAX_COLS * (MAX_ROWS - 1); i++)
    {
        // Copy the character from the next line
        putchar(i % MAX_COLS, i / MAX_COLS, getchar(i % MAX_COLS, i / MAX_COLS + 1), getcolor(i % MAX_COLS, i / MAX_COLS + 1));
    }

    // Clear the last line
    for (int i = 0; i < MAX_COLS; i++)
    {
        putchar(i, MAX_ROWS - 1, ' ', DEFAULT_COLOR);
    }

    // Reset the cursor position
    cursor_x = 0;
    cursor_y = MAX_ROWS - 1;
    set_cursor(cursor_x, cursor_y);
}

void putc(char c)
{
    // Write a character to the screen
    switch (c)
    {
    case '\n':
        // Newline
        cursor_x = 0;
        cursor_y++;
        break;
    case '\t':
        // Tab
        cursor_x += 4;
        break;
    case '\r':
        // Carriage return
        cursor_x = 0;
        break;
    default:
        // Write the character to the screen
        putchar(cursor_x, cursor_y, c, DEFAULT_COLOR);
        cursor_x++;
        break;
    }

    // Check if the cursor is out of bounds
    if (cursor_x >= MAX_COLS)
    {
        // Newline
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= MAX_ROWS)
    {
        // Scroll the screen
        scroll();
    }

    // Update the cursor position
    set_cursor(cursor_x, cursor_y);
}

void puts(const char *text)
{
    // Write a string to the screen
    while (*text)
    {
        putc(*text);
        text++;
    }
}

void vsprintf(char *str, const char *fmt, va_list args)
{
    // Format a string
    int i = 0;
    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 'd':
                // Integer
                i += itoa(va_arg(args, int), str + i, 10);
                break;
            case 'x':
                // Hexadecimal
                i += itoa(va_arg(args, int), str + i, 16);
                break;
            case 's':
                // String
                i += strcpy(str + i, va_arg(args, char *));
                break;
            case 'c':
                // Character
                str[i++] = va_arg(args, int);
                break;
            case 'f':
                // Float
                i += ftoa(va_arg(args, double), str + i, 2);
                break;
            }
        }
        else
        {
            // Copy the character
            str[i++] = *fmt;
        }
        fmt++;
    }
    str[i] = '\0';
}

void vprintf(const char *fmt, va_list args)
{
    // Format a string and print it
    char str[1024];
    vsprintf(str, fmt, args);
    puts(str);
}

void sprintf(char *str, const char *fmt, ...)
{
    // Format a string
    va_list args;
    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);
}

void printf(const char *fmt, ...)
{
    // Format a string and print it
    va_list args;
    va_start(args, fmt);
    char str[1024];
    vsprintf(str, fmt, args);
    va_end(args);
    puts(str);
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
