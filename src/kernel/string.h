#ifndef STRING_H
#define STRING_H
#include <types.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, u8 val, size_t count);
void *memmove(void *dest, const void *src, size_t count);

size_t strlen(const char *s);
int strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

int itoa(int value, char *buffer, int base);
int ftoa(float value, char *buffer, int afterpoint);

#endif