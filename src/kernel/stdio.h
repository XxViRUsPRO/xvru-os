#ifndef STDIO_H
#define STDIO_H
#include <types.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define DEFAULT_COLOR 0x07

static u8 cursor_x = 0;
static u8 cursor_y = 0;
static u8 *vidmem = (u8 *)VIDEO_ADDRESS;

void clear_screen();
void putc(char c);
void puts(const char *str);
void sprintf(char *str, const char *fmt, ...);
void printf(const char *fmt, ...);

#endif