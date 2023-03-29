#ifndef _STRING_H_
#define _STRING_H_
#include <types.h>

size_t strlen(const char *s);
int strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

int itoa(int value, char *buffer, int base);
int ftoa(float value, char *buffer, int afterpoint);

#endif