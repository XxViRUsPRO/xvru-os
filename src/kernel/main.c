#include "stdio.h"
#include "string.h"
#include "magic.h"
#include "math.h"
#include <hal/hal.h>
#include <types.h>

void _main32(void)
{
    HAL_init();
    clear_screen();
    printf("Hello, world!\n");
    printf("PI = %f\n", PI);

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
