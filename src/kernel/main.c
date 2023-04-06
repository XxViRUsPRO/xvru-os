#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>

typedef struct
{
    vec2d o;
    vec2d d;
} ray;

void _main32(void)
{
    HAL_init();
    vec2d origin = {0, 0};
    i32 radius = 20;

    // Draw circle
    for (i32 x = -radius; x <= radius; x++)
    {
        for (i32 y = -radius; y <= radius; y++)
        {
            if (x * x + y * y <= radius * radius)
            {
                draw_pixel((vec2d){x, y}, LIGHT_BLUE);
            }
        }
    }

    ray r[] = {
        {{0, 0}, {1, 0}},
        {{0, 0}, {1, 1}},
        {{0, 0}, {0, 1}},
        {{0, 0}, {-1, 1}},
        {{0, 0}, {-1, 0}},
        {{0, 0}, {-1, -1}},
        {{0, 0}, {0, -1}},
        {{0, 0}, {1, -1}},
    };

    for (u32 i = 0; i < sizeof(r) / sizeof(ray); i++)
    {
        vec2d p = intersect_circle(r[i].o, r[i].d, origin, radius);
        // draw a 2x2 pixel square
        if (p.x != MAX_INT && p.y != MIN_INT)
        {
            draw_pixel(p, RED);
            draw_pixel((vec2d){p.x + 1, p.y}, RED);
            draw_pixel((vec2d){p.x, p.y + 1}, RED);
            draw_pixel((vec2d){p.x + 1, p.y + 1}, RED);
        }
    }

    refresh_screen();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
