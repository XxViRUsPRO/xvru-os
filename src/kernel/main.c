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
    vec2d center = {-10, -10};
    vec2d center2 = {0, 0};
    vec2d center3 = {10, 10};
    i32 radius = 50;

    // Draw a circle
    for (i32 o = 0; o < 360; o++)
    {
        vec2d p = {
            (i32)(center.x + radius * cos(o)),
            (i32)(center.y + radius * sin(o))};
        draw_pixel(p, WHITE);
    }
    for (i32 o = 0; o < 360; o++)
    {
        vec2d p = {
            (i32)(center2.x + radius * cos(o)),
            (i32)(center2.y + radius * sin(o))};
        draw_pixel(p, RED);
    }
    for (i32 o = 0; o < 360; o++)
    {
        vec2d p = {
            (i32)(center3.x + radius * cos(o)),
            (i32)(center3.y + radius * sin(o))};
        draw_pixel(p, GREEN);
    }

    render();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
