#include "hal.h"
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/isr.h>

void HAL_init(void)
{
    GDT_init();
    IDT_init();
    ISR_init();
}
