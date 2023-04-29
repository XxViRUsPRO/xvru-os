#ifndef DISPLAY_H
#define DISPLAY_H
#include <types.h>
#include <math.h>

#define VGA_MEM 0xfd000000
#define SCREEN_W 800
#define SCREEN_H 600
#define MAX_X (SCREEN_W / 2) - 1
#define MAX_Y (SCREEN_H / 2) - 1
#define MIN_X -MAX_X
#define MIN_Y -MAX_Y

#define PIXEL_OFFSET(x, y, pitch, bpp) (y * (pitch / (bpp / 8)) + x)
#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))

static u32 *vga_memory = (u32 *)VGA_MEM;
static u32 vga_buffer[2][SCREEN_W * SCREEN_H] = {0};

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
} triangle_t;

typedef struct
{
    triangle_t t[12];
} mesh_t;

typedef struct
{
    f32 m[4][4];
} mat4x4;

#define VEC2D(x, y) ((vec2d){x, y})
#define VEC3D(x, y, z) ((vec3d){x, y, z})

void draw_pixel(vec2d p, vec3d color);
void put_pixel(vec2d p, vec3d color);
void draw_char(char c, u8 size, vec2d pos, vec3d color);
void draw_string(const char *str, u8 size, vec2d pos, vec3d color);
void draw_line(vec2d v1, vec2d v2, vec3d color);
void draw_triangle(triangle_t *t, u32 size, vec3d color);
void fill_triangle(triangle_t *t, u32 size, vec3d color);
void swap_buffers();
void fill_vga_buffer(u32 value);
void clear_vga_buffer();
void render();
vec2d to_screen_space(vec2d p);

#endif