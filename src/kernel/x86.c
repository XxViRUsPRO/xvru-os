#include "x86.h"
#include <stdio.h>

#define UNUSED_PORT 0x80

void io_wait()
{
    outb(UNUSED_PORT, 0);
}

void __panic(const char *msg)
{
    fprintf(DEBUG_FD, "PANIC: %s\n", msg);
    __asm__ __volatile__("cli; hlt");
}
