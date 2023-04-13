#ifndef IRQ_H
#define IRQ_H
#include <types.h>
#include "isr.h"

typedef void (*IRQ_handler_t)(Registers *regs);

void IRQ_init();
void IRQ_install(u8 irq, IRQ_handler_t handler);

#endif