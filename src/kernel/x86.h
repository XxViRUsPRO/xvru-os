#ifndef MAGIC_H
#define MAGIC_H
#include <types.h>

#define _cdecl __attribute__((cdecl))

_cdecl void outb(u16 port, u8 value);
_cdecl u8 inb(u16 port);
_cdecl void outw(u16 port, u16 value);
_cdecl u16 inw(u16 port);

void panic();
void cli();
void sti();
void io_wait();
void x86_enable_paging();
void x86_load_pdbr(void *pdbr);

// Register structs for interrupt/exception
typedef struct registers
{
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} register_t;

// Register structs for bios service
typedef struct register16
{
    u16 di;
    u16 si;
    u16 bp;
    u16 sp;
    u16 bx;
    u16 dx;
    u16 cx;
    u16 ax;

    u16 ds;
    u16 es;
    u16 fs;
    u16 gs;
    u16 ss;
    u16 eflags;
} register16_t;

#endif