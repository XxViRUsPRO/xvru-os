#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <types.h>

#define VGA_MEM 0xA0000
#define MAX_X 320
#define MAX_Y 200

static u8 *vga_memory = (u8 *)VGA_MEM;
static u8 vga_buffer[200][320] = {0};

enum vga_color
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
};

typedef struct
{
    u16 x;
    u16 y;
} vec2d;

typedef struct
{
    u16 x, y, z;
} vec3d;

void draw_pixel(vec2d p, enum vga_color color);
void draw_line(vec2d v1, vec2d v2, enum vga_color color);
void refresh_screen();
vec2d to_screen_space(vec2d p);
vec2d intersect_circle(vec2d origin, vec2d direction, vec2d center, f32 radius);

#endif