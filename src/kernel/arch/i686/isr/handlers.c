#include "handlers.h"
#include <stdio.h>
#include <x86.h>

void ISR_handler_divide_by_zero(Registers *regs)
{
    printf("Divide by zero!\n");
    PANIC("Divide by zero");
}

void ISR_handler_page_fault(Registers *regs)
{
    u32 faulting_address;
    __asm__ __volatile__("mov %%cr2, %0"
                         : "=r"(faulting_address));

    u32 present = !(regs->err_code & 0x1);
    u32 rw = regs->err_code & 0x2;
    u32 us = regs->err_code & 0x4;
    u32 reserved = regs->err_code & 0x8;
    u32 id = regs->err_code & 0x10;

    printf("Page fault! ( ");
    if (present)
        printf("present ");
    if (rw)
        printf("read-only ");
    if (us)
        printf("user-mode ");
    if (reserved)
        printf("reserved ");
    printf(") at 0x%x\n", faulting_address);
    PANIC("Page fault");
}

void ISR_handlers_init()
{
    ISR_install(0, ISR_handler_divide_by_zero);
    ISR_install(14, ISR_handler_page_fault);
}