#ifndef _STRING_H_
#define _STRING_H_
#include "types.h"

size_t strlen(const char *s);

char *itoa(int value, char *buffer, int base);

#endif