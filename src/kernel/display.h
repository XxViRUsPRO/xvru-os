#pragma once
#include <stdint.h>

#define VGA_ADDRESS 0xA0000

enum VGA_MODES
{
    VGA_MODE_320x200x256 = 0x13,
};

void putc_vga(char c, uint32_t x, uint32_t y, uint8_t color, uint8_t *buffer = (uint8_t *)VGA_ADDRESS);
void puts_vga(const char *str, uint32_t x, uint32_t y, uint8_t color, uint8_t *buffer = (uint8_t *)VGA_ADDRESS);
void bezier_curve(int x[4], int y[4], uint8_t color, uint8_t *buffer = (uint8_t *)VGA_ADDRESS);
