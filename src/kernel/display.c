#include <display.h>
#include <string.h>
#include <font.h>

static u32 vga_buffer_ptr = 0;

void draw_pixel(vec2d v, vec3d color)
{
    vec2d p = to_screen_space(v);
    if (p.x < 0 || p.x >= SCREEN_W / 2 || p.y < 0 || p.y >= SCREEN_H / 2)
        return;
    put_pixel(p, color);
}

void put_pixel(vec2d p, vec3d color)
{
    u8 red = (u8)(color.x);
    u8 green = (u8)(color.y);
    u8 blue = (u8)(color.z);
    u32 *pixel = (u32 *)(vga_buffer[vga_buffer_ptr] + (int)(p.y * SCREEN_W + p.x));
    *pixel = COLOR(red, green, blue);
}

void draw_char(char c, u8 size, vec2d pos, vec3d color)
{
    u8 *glyph = font8x8_basic[c];
    u8 x, y;
    for (y = 0; y < 8 * size; y++)
    {
        for (x = 0; x < 8 * size; x++)
        {
            if ((glyph[y / size] >> (x / size)) & 1)
                put_pixel(VEC2D(pos.x + x, pos.y + y), color);
        }
    }
}

void draw_string(const char *str, u8 size, vec2d pos, vec3d color)
{
    u8 i = 0;
    while (str[i])
    {
        draw_char(str[i], size, VEC2D(pos.x + (i * 8 * size), pos.y), color);
        i++;
    }
}

// DDA algorithm
void draw_line(vec2d v1, vec2d v2, vec3d color)
{
    i32 dx, dy, steps, i;
    f32 xinc, yinc, x, y;

    dx = v2.x - v1.x;
    dy = v2.y - v1.y;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    xinc = dx / (f32)steps;
    yinc = dy / (f32)steps;

    x = v1.x;
    y = v1.y;

    for (i = 0; i <= steps; i++)
    {
        draw_pixel((vec2d){(i32)x, (i32)y}, color);
        x += xinc;
        y += yinc;
    }
}

void draw_triangle(triangle_t *t, u32 size, vec3d color)
{
    draw_line((vec2d){t->p[0].x * size, t->p[0].y * size}, (vec2d){t->p[1].x * size, t->p[1].y * size}, color);
    draw_line((vec2d){t->p[1].x * size, t->p[1].y * size}, (vec2d){t->p[2].x * size, t->p[2].y * size}, color);
    draw_line((vec2d){t->p[2].x * size, t->p[2].y * size}, (vec2d){t->p[0].x * size, t->p[0].y * size}, color);
}

void fill_triangle(triangle_t *t, u32 size, vec3d color)
{
    vec2d p1 = (vec2d){t->p[0].x * size, t->p[0].y * size};
    vec2d p2 = (vec2d){t->p[1].x * size, t->p[1].y * size};
    vec2d p3 = (vec2d){t->p[2].x * size, t->p[2].y * size};

    vec2d min = (vec2d){MIN(MIN(p1.x, p2.x), p3.x), MIN(MIN(p1.y, p2.y), p3.y)};
    vec2d max = (vec2d){MAX(MAX(p1.x, p2.x), p3.x), MAX(MAX(p1.y, p2.y), p3.y)};

    for (i32 x = (i32)min.x; x <= (i32)max.x; x++)
    {
        for (i32 y = (i32)min.y; y <= (i32)max.y; y++)
        {
            vec2d p = (vec2d){x, y};
            f32 w0 = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
            f32 w1 = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
            f32 w2 = 1.0f - w0 - w1;
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                draw_pixel(p, color);
            }
        }
    }
}

void fill_vga_buffer(u32 value)
{
    memset(vga_buffer[vga_buffer_ptr], value, SCREEN_W * SCREEN_H * sizeof(u32));
}

void clear_vga_buffer()
{
    fill_vga_buffer(0);
}

void swap_buffers()
{
    vga_buffer_ptr = (vga_buffer_ptr + 1) % 2;
}

void render()
{
    // TODO: We can optimize this by only rendering the pixels that have changed since the last render by using changes queue
    memcpy(vga_memory, vga_buffer[vga_buffer_ptr], SCREEN_W * SCREEN_H * sizeof(u32));
    swap_buffers();
}

vec2d to_screen_space(vec2d p)
{
    return (vec2d){p.x + SCREEN_W / 2, SCREEN_H / 2 - p.y};
}
