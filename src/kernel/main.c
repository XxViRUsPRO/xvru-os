#include "stdio.h"
#include "string.h"
#include "magic.h"
#include <hal/hal.h>
#include <types.h>

void _main32(void)
{
    HAL_init();
    clear_screen();
    printf("Hello, world! %f\n", 32.45f);

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
