#ifndef STDIO_H
#define STDIO_H
#include <types.h>
#include <hal/vfs.h>
#include <stdarg.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define DEFAULT_COLOR 0x07
static u8 cursor_x = 0;
static u8 cursor_y = 0;
static u8 *vidmem = (u8 *)VIDEO_ADDRESS;

void fputc(fd_t fd, char c);
void fputs(fd_t fd, const char *str);
void vfprintf(fd_t fd, const char *fmt, va_list arg);
void fprintf(fd_t fd, const char *fmt, ...);

void putc(char c);
void puts(const char *str);
void printf(const char *fmt, ...);

int itoa(int value, char *buffer, int base);
int atoi(const char *str);
int ftoa(float value, char *buffer, int afterpoint);

#endif