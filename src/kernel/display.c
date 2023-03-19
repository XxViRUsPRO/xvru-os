#include "display.h"
#include "magic.h"

void setup_text(void)
{
    // Clear the screen
    clear_text();

    // Disable the cursor
    disable_cursor();
}

void clear_text(void)
{
    // Clear the screen
    for (int i = 0; i < 80 * 25; i++)
    {
        // Write a space to the screen
        write_char(' ', i, 0x07);
    }

    // Reset the cursor position
    set_cursor(0, 0);
}

void write_char(const char c, int pos, char color)
{
    // Write a character to the screen
    char *vidmem = (char *)VIDEO_ADDRESS;
    vidmem[pos * 2] = c;
    vidmem[pos * 2 + 1] = color;
}

void write_text(const char *text, int pos, char color)
{
    // Write a string to the screen
    while (*text)
    {
        write_char(*text, pos, color);
        text++;
        pos++;
    }
}

void set_cursor(int x, int y)
{
    // Set the cursor position
    unsigned short pos = y * 80 + x;

    // Send the high byte
    outb(0x3D4, 14);
    outb(0x3D5, (unsigned char)(pos >> 8));

    // Send the low byte
    outb(0x3D4, 15);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
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
