#ifndef STRING_H
#define STRING_H
#include "types.h"

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, i32 val, size_t count);
void *memmove(void *dest, const void *src, size_t count);
int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *s);
int strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
char *strchr(const char *str, int c);

#endif