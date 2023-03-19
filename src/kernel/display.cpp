#include <stdint.h>
#include "display.h"
#include "font.h"
#include "math.h"

void putc_vga(char c, uint32_t x, uint32_t y, uint8_t color, uint8_t *buffer)
{
    uint8_t *a = font8x8_basic[c];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (a[i] & (1 << j))
            {
                buffer[y * 320 + x] = color;
            }
            x++;
        }
        x -= 8;
        y++;
    }
}

void puts_vga(const char *str, uint32_t x, uint32_t y, uint8_t color, uint8_t *buffer)
{
    while (*str)
    {
        putc_vga(*str, x, y, color, buffer);
        x += 8;
        str++;
    }
}

void bezier_curve(int x[4], int y[4], uint8_t color, uint8_t *buffer)
{
    double xu = 0, yu = 0, u = 0;
    for (u = 0; u <= 1; u += 0.0001)
    {
        xu = pow(1 - u, 3) * x[0] + 3 * u * pow(1 - u, 2) * x[1] + 3 * pow(u, 2) * (1 - u) * x[2] + pow(u, 3) * x[3];
        yu = pow(1 - u, 3) * y[0] + 3 * u * pow(1 - u, 2) * y[1] + 3 * pow(u, 2) * (1 - u) * y[2] + pow(u, 3) * y[3];
        buffer[(int)yu * 320 + (int)xu] = color;
    }
}
