#ifndef STDIO_H
#define STDIO_H
#include <types.h>
#include <stdarg.h>
#include <hal/vfs.h>

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