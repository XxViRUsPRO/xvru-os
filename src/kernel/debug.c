#include "debug.h"
#include "types.h"

inline void outb(u16 port, u8 val)
{
    __asm__ __volatile__("outb %0, %1"
                         :
                         : "a"(val), "Nd"(port));
}

void dbg_putc(char c)
{
    outb(0xE9, c);
}

void dbg_puts(const char *s)
{
    while (*s)
        dbg_putc(*s++);
}