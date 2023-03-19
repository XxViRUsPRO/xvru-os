#include "types.h"
#include "display.h"

void _main32(void)
{
    setup_text();
    write_text("Hello and welcome to XVRU OS!", 0, 0x02);

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
