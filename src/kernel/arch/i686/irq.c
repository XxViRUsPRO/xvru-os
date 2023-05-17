#include "irq.h"
#include "pic.h"
#include <x86.h>
#include <stdio.h>

#define PIC_REMAP_OFFSET 0x20

IRQ_handler_t IRQ_handlers[16];

void IRQ_handler(Registers *regs)
{
    i32 irq = regs->int_no - PIC_REMAP_OFFSET;

    u8 pic_isr = PIC_get_isr_reg();
    u8 pic_irr = PIC_get_irr_reg();

    if (IRQ_handlers[irq] != NULL)
        IRQ_handlers[irq](regs);
    else
        printf("Unhandled IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);
    PIC_send_EOI(irq);
}

void IRQ_init()
{
    PIC_config(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    for (u8 i = 1; i < 16; i++)
    {
        ISR_install(PIC_REMAP_OFFSET + i, IRQ_handler);
    }

    sti();
}

void IRQ_install(u8 irq, IRQ_handler_t handler)
{
    cli();
    IRQ_handlers[irq] = handler;
    PIC_mask(irq, false);
    sti();
}