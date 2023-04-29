#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>
#include <drivers/timer.h>
#include <drivers/vesa.h>

const char *msg = "Hello, world!";

void __attribute__((section(".entry"))) kmain(VbeModeInfoBlock *mode_info)
{
    HAL_init();
    timer_init();
    clear_vga_buffer();
    draw_string(msg, 2, VEC2D(0, 0), VEC3D(255, 255, 255));
    render();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
