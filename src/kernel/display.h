#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <types.h>

#define VGA_MEM 0xA0000
#define SCREEN_W 320
#define SCREEN_H 200
#define MAX_X (SCREEN_W / 2) - 1
#define MAX_Y (SCREEN_H / 2) - 1
#define MIN_X -MAX_X
#define MIN_Y -MAX_Y

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
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15,
};

typedef struct
{
    i32 x;
    i32 y;
} vec2d;

typedef struct
{
    i32 x, y, z;
} vec3d;

void draw_pixel(vec2d p, enum vga_color color);
void draw_line(vec2d v1, vec2d v2, enum vga_color color);
void render();
vec2d to_screen_space(vec2d p);
vec2d intersect_circle(vec2d origin, vec2d direction, vec2d center, f32 radius);

#endif