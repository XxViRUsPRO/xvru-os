#include <display.h>
#include <string.h>

void draw_pixel(vec2d v, u8 color)
{
    vec2d p = to_screen_space(v);
    vga_buffer[(i32)p.y][(i32)p.x] = color;
}

// DDA algorithm
void draw_line(vec2d v1, vec2d v2, u8 color)
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

void draw_triangle(triangle_t *t, u32 size, u8 color)
{
    draw_line((vec2d){t->p[0].x * size, t->p[0].y * size}, (vec2d){t->p[1].x * size, t->p[1].y * size}, color);
    draw_line((vec2d){t->p[1].x * size, t->p[1].y * size}, (vec2d){t->p[2].x * size, t->p[2].y * size}, color);
    draw_line((vec2d){t->p[2].x * size, t->p[2].y * size}, (vec2d){t->p[0].x * size, t->p[0].y * size}, color);
}

void fill_triangle(triangle_t *t, u32 size, u8 color)
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

void clear_vga_buffer()
{
    memset(vga_buffer, 0, SCREEN_W * SCREEN_H);
}

void render()
{
    // TODO: We can optimize this by only rendering the pixels that have changed since the last render by using changes queue
    memcpy(vga_memory, vga_buffer, SCREEN_W * SCREEN_H);
}

vec2d to_screen_space(vec2d p)
{
    return (vec2d){p.x + SCREEN_W / 2, SCREEN_H / 2 - p.y};
}
