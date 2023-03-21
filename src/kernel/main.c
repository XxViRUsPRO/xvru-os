#include "types.h"
#include "display.h"
#include "string.h"
#include "magic.h"
#include <arch/i386/hal.h>

void _main32(void)
{
    test();
    setup_text();
    write_text("Hello and welcome to XVRU OS!", 0, 0x02);
    write_text("Counter: ", (MAX_COLS) * (MAX_ROWS - 1), 0x02);
    u32 count = 0;
    while (true)
    {
        char buffer[100];
        itoa(count++, buffer, 10);
        write_text(buffer, (MAX_COLS) * (MAX_ROWS - 1) + 9, 0x03);
        sleep(1000000);
    }

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
