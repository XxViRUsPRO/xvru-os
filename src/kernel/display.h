#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <types.h>
#include <math.h>

#define VGA_MEM 0xA0000
#define SCREEN_W 320
#define SCREEN_H 200
#define MAX_X (SCREEN_W / 2) - 1
#define MAX_Y (SCREEN_H / 2) - 1
#define MIN_X -MAX_X
#define MIN_Y -MAX_Y

#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GRAY 7
#define VGA_COLOR_DARK_GRAY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_YELLOW 14
#define VGA_COLOR_WHITE 15

static u8 *vga_memory = (u8 *)VGA_MEM;
static u8 vga_buffer[200][320] = {0};

typedef struct
{
    f32 x;
    f32 y;
} vec2d;

typedef struct
{
    f32 x, y, z;
} vec3d;

typedef struct
{
    vec3d p[3];
} triangle;

typedef struct
{
    triangle t[12];
} cube_mesh;

typedef struct
{
    f32 m[4][4];
} mat4x4;

void draw_pixel(vec2d p, u8 color);
void draw_line(vec2d v1, vec2d v2, u8 color);
void draw_triangle(triangle *t, u8 color);
void fill_triangle(triangle *t, u8 color);
void clear_vga_buffer();
void render();
vec2d to_screen_space(vec2d p);
vec2d intersection(vec2d origin, vec2d direction, vec2d center, f32 radius);

#endif