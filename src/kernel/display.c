#include "display.h"
#include "memory.h"

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

void draw_triangle(triangle *t, u8 color)
{
    draw_line((vec2d){t->p[0].x * 100, t->p[0].y * 100}, (vec2d){t->p[1].x * 100, t->p[1].y * 100}, color);
    draw_line((vec2d){t->p[1].x * 100, t->p[1].y * 100}, (vec2d){t->p[2].x * 100, t->p[2].y * 100}, color);
    draw_line((vec2d){t->p[2].x * 100, t->p[2].y * 100}, (vec2d){t->p[0].x * 100, t->p[0].y * 100}, color);
}

void fill_triangle(triangle *t, u8 color)
{
    vec2d p1 = (vec2d){t->p[0].x * 100, t->p[0].y * 100};
    vec2d p2 = (vec2d){t->p[1].x * 100, t->p[1].y * 100};
    vec2d p3 = (vec2d){t->p[2].x * 100, t->p[2].y * 100};

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
    return (vec2d){(i32)abs(p.x + MAX_X) % SCREEN_W, (i32)abs(MAX_Y - p.y) % SCREEN_H};
}

/**
 * @brief Calculate the intersection of a ray with a circle
 *
 * @param origin The origin of the ray
 * @param direction The direction of the ray
 * @param center The center of the circle
 * @param radius The radius of the circle
 * @return vec2d The nearst intersection point | (MAX_INT, MIN_INT) if there is no intersection
 */
vec2d intersection(vec2d origin, vec2d direction, vec2d center, f32 radius)
{
    i32 a = direction.x * direction.x + direction.y * direction.y;
    i32 b = 2 * (direction.x * (origin.x - center.x) + direction.y * (origin.y - center.y));
    i32 c = (origin.x - center.x) * (origin.x - center.x) + (origin.y - center.y) * (origin.y - center.y) - radius * radius;
    i32 discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return (vec2d){NAN, NAN};
    }
    f32 t1 = (-b + sqrt(discriminant)) / (2 * a);
    f32 t2 = (-b - sqrt(discriminant)) / (2 * a);
    f32 t = MIN(t1, t2);
    return (vec2d){origin.x + direction.x * t, origin.y + direction.y * t};
}
