#include <stdint.h>
#include "std.h"
#include "display.h"
#include "memory.h"
#include "math.h"
#include "x86.h"

#define ANGLE 45
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

double dot(double a[3], double b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void draw_line(int x1, int y1, int x2, int y2, uint8_t color, uint8_t *buffer)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    while (true)
    {
        buffer[y1 * 320 + x1] = color;
        if (x1 == x2 && y1 == y2)
        {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

extern "C" void
_main32(void)
{
    // Initialize the vga buffer
    uint8_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]{};

    // Cube vertices
    double vertices[8][3] = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 1},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 0},
        {1, 1, 1},
    };

    // projection matrix x, y, z to x, y
    double projection[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
    };

    // rotation matrix with angle ANGLE degrees
    // rotate y
    double rotation[3][3] = {
        {cos(ANGLE), 0, sin(ANGLE)},
        {0, 1, 0},
        {-sin(ANGLE), 0, cos(ANGLE)},
    };
    // rotate z
    double rotation2[3][3] = {
        {cos(ANGLE), -sin(ANGLE), 0},
        {sin(ANGLE), cos(ANGLE), 0},
        {0, 0, 1},
    };
    // rotate x
    double rotation3[3][3] = {
        {1, 0, 0},
        {0, cos(ANGLE), -sin(ANGLE)},
        {0, sin(ANGLE), cos(ANGLE)},
    };

    for (int i = 0; i < 8; i++)
    {
        // rotate x, y, z
        double temp[3] = {
            dot(rotation[0], vertices[i]),
            dot(rotation[1], vertices[i]),
            dot(rotation[2], vertices[i]),
        };
        temp[0] = dot(rotation2[0], temp);
        temp[1] = dot(rotation2[1], temp);
        temp[2] = dot(rotation2[2], temp);
        temp[0] = dot(rotation3[0], temp);
        temp[1] = dot(rotation3[1], temp);
        temp[2] = dot(rotation3[2], temp);
        vertices[i][0] = temp[0];
        vertices[i][1] = temp[1];
        vertices[i][2] = temp[2];
    }

    // project to 2d
    for (int i = 0; i < 8; i++)
    {
        double temp[3] = {
            dot(projection[0], vertices[i]),
            dot(projection[1], vertices[i]),
            dot(projection[2], vertices[i]),
        };
        vertices[i][0] = temp[0];
        vertices[i][1] = temp[1];
        vertices[i][2] = temp[2];
    }

    // draw cube and fill outer faces
    draw_line(vertices[0][0] * 50 + 120, vertices[0][1] * 50 + 80, vertices[1][0] * 50 + 120, vertices[1][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[0][0] * 50 + 120, vertices[0][1] * 50 + 80, vertices[2][0] * 50 + 120, vertices[2][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[0][0] * 50 + 120, vertices[0][1] * 50 + 80, vertices[4][0] * 50 + 120, vertices[4][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[1][0] * 50 + 120, vertices[1][1] * 50 + 80, vertices[3][0] * 50 + 120, vertices[3][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[1][0] * 50 + 120, vertices[1][1] * 50 + 80, vertices[5][0] * 50 + 120, vertices[5][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[2][0] * 50 + 120, vertices[2][1] * 50 + 80, vertices[3][0] * 50 + 120, vertices[3][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[2][0] * 50 + 120, vertices[2][1] * 50 + 80, vertices[6][0] * 50 + 120, vertices[6][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[3][0] * 50 + 120, vertices[3][1] * 50 + 80, vertices[7][0] * 50 + 120, vertices[7][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[4][0] * 50 + 120, vertices[4][1] * 50 + 80, vertices[5][0] * 50 + 120, vertices[5][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[4][0] * 50 + 120, vertices[4][1] * 50 + 80, vertices[6][0] * 50 + 120, vertices[6][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[5][0] * 50 + 120, vertices[5][1] * 50 + 80, vertices[7][0] * 50 + 120, vertices[7][1] * 50 + 80, 0x0F, (uint8_t *)buffer);
    draw_line(vertices[6][0] * 50 + 120, vertices[6][1] * 50 + 80, vertices[7][0] * 50 + 120, vertices[7][1] * 50 + 80, 0x0F, (uint8_t *)buffer);

    memcpy((uint8_t *)VGA_ADDRESS, (uint8_t *)buffer, SCREEN_HEIGHT * SCREEN_WIDTH);

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
