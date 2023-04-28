#ifndef STRING_H
#define STRING_H
#include <types.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, u8 val, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *s1, const void *s2, size_t n);
void *memchr(const void *str, int c, size_t n);

size_t strlen(const char *s);
int strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
char *strchr(const char *s, int c);

#endif