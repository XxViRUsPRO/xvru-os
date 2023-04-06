#include "display.h"
#include "math.h"

void draw_pixel(vec2d v, enum vga_color color)
{
    vec2d p = to_screen_space(v);
    if (p.x < 0 || p.x > MAX_X || p.y < 0 || p.y > MAX_Y)
    {
        return;
    }
    vga_buffer[p.y][p.x] = color;
}

// DDA algorithm
void draw_line(vec2d v1, vec2d v2, enum vga_color color)
{
    i32 dx, dy, steps, i;
    f32 xinc, yinc, x, y;
    dx = v2.x - v1.x;
    dy = v2.y - v1.y;
    if (abs(dx) > abs(dy))
    {
        steps = abs(dx);
        xinc = (f32)dx / (f32)steps;
        yinc = (f32)dy / (f32)steps;
        x = v1.x;
        y = v1.y;
        for (i = 0; i <= steps; i++)
        {
            draw_pixel((vec2d){(i32)x, (i32)y}, color);
            x += xinc;
            y += yinc;
        }
    }
    else
    {
        steps = abs(dy);
        xinc = (f32)dx / (f32)steps;
        yinc = (f32)dy / (f32)steps;
        x = v1.x;
        y = v1.y;
        for (i = 0; i <= steps; i++)
        {
            draw_pixel((vec2d){(i32)x, (i32)y}, color);
            x += xinc;
            y += yinc;
        }
    }
}

void refresh_screen()
{
    for (u16 y = 0; y < MAX_Y; y++)
    {
        u16 i = y * MAX_X;
        for (u16 x = 0; x < MAX_X; x++)
        {
            vga_memory[i + x] = vga_buffer[y][x];
        }
    }
}

vec2d to_screen_space(vec2d p)
{
    return (vec2d){p.x + MAX_X / 2, MAX_Y / 2 - p.y};
}

/**
 * @brief Calculate the intersection of a ray with a circle
 *
 * @param origin The origin of the ray
 * @param direction The direction of the ray
 * @param center The center of the circle
 * @param radius The radius of the circle
 * @return vec2d The intersection point | (MAX_INT, MIN_INT) if there is no intersection
 */
vec2d intersect_circle(vec2d origin, vec2d direction, vec2d center, f32 radius)
{
    i32 a = direction.x * direction.x + direction.y * direction.y;
    i32 b = 2 * (direction.x * (origin.x - center.x) + direction.y * (origin.y - center.y));
    i32 c = (origin.x - center.x) * (origin.x - center.x) + (origin.y - center.y) * (origin.y - center.y) - radius * radius;
    i32 discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return (vec2d){MAX_INT, MIN_INT};
    }
    f32 t1 = (-b + isqrt(discriminant)) / (2 * a);
    f32 t2 = (-b - isqrt(discriminant)) / (2 * a);
    f32 t = MIN(t1, t2);
    return (vec2d){origin.x + direction.x * t, origin.y + direction.y * t};
}
