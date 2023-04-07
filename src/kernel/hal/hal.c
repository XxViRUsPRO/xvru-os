#include "hal.h"
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/isr.h>

void HAL_init(void)
{
    GDT_init();
    IDT_init();
    ISR_init();
}
