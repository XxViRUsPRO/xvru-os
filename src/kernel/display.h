#ifndef DISPLAY_H
#define DISPLAY_H
#include <types.h>

#define PIXEL_OFFSET(x, y) ((x) + (y)*g_Pitch)
#define COLOR(r, g, b) ((r) << 16 | (g) << 8 | (b))

void display_init(void *framebuffer, u32 width, u32 height, u32 pitch, u32 bpp);
void display_clear(u32 color);
void display_draw_pixel(u32 x, u32 y, u32 color);
void display_draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color);
void display_draw_char(u32 x, u32 y, char c, u32 color);
void display_render();
void display_swap_buffers();
void display_destroy();

#endif