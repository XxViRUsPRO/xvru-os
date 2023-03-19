#pragma once
#include <stdint.h>

int rand(void);
void srand(uint32_t seed);
int strlen(const char *str);
char *itoa(int value, char *str, int base);
