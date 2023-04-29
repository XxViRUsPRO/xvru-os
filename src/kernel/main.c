#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include "debug.h"
#include "display.h"
#include <hal/hal.h>
#include <types.h>
#include <drivers/timer.h>
#include <drivers/vesa.h>

void __attribute__((section(".entry"))) kmain(VbeModeInfoBlock *mode_info)
{
    HAL_init();
    timer_init();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
