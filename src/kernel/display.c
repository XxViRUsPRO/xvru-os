#include "display.h"
#include <stdlib.h>
#include <string.h>

static void *g_FrameBuffer = NULL;
static void *g_DoubleBuffer = NULL;
static u32 g_BufferSize = 0;
static u32 g_DoubleBufferIndex = 0;
static u32 g_Width = 0;
static u32 g_Height = 0;
static u32 g_Pitch = 0;
static u32 g_BitsPerPixel = 0;
static u32 g_SizeOfPixel = 0;

void display_init(void *framebuffer, u32 width, u32 height, u32 pitch, u32 bpp)
{
    if (framebuffer == NULL)
    {
        return;
    }

    g_FrameBuffer = framebuffer;
    g_SizeOfPixel = bpp / 8;
    g_BufferSize = width * height * g_SizeOfPixel;
    // g_DoubleBuffer = malloc(g_BufferSize * 2);
    g_DoubleBuffer = g_FrameBuffer;
    if (g_DoubleBuffer == NULL)
    {
        return;
    }

    g_Width = width;
    g_Height = height;
    g_Pitch = pitch;
    g_BitsPerPixel = bpp;
}

void display_clear(u32 color)
{
    memset(g_DoubleBuffer + g_DoubleBufferIndex, color, g_BufferSize);
}

void display_draw_pixel(u32 x, u32 y, u32 color)
{
    if (x >= g_Width || y >= g_Height)
    {
        return;
    }

    u32 *pixel = (u32 *)g_DoubleBuffer + PIXEL_OFFSET(x, y);
    *pixel = color;
}

void display_draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color)
{
    if (x >= g_Width || y >= g_Height)
    {
        return;
    }

    u32 *pixel = (u32 *)g_DoubleBuffer + PIXEL_OFFSET(x, y);
    for (u32 i = 0; i < height; i++)
    {
        for (u32 j = 0; j < width; j++)
        {
            pixel[j] = color;
        }
        pixel += g_Pitch / g_SizeOfPixel;
    }
}

void display_draw_char(u32 x, u32 y, char c, u32 color)
{
}

void display_render()
{
    memcpy(g_FrameBuffer, g_DoubleBuffer + g_DoubleBufferIndex, g_BufferSize);
}

void display_swap_buffers()
{
    g_DoubleBufferIndex = (g_DoubleBufferIndex + g_BufferSize) % (2 * g_BufferSize);
}

void display_destroy()
{
    if (g_DoubleBuffer != NULL)
    {
        free(g_DoubleBuffer);
    }
}
