#ifndef PIC_H
#define PIC_H
#include <types.h>

void PIC_config(u8 offsetPic1, u8 offsetPic2);
void PIC_send_EOI(u8 irq);
void PIC_mask(u8 irq, bool set);
void PIC_disable();
u16 PIC_get_irr_reg();
u16 PIC_get_isr_reg();

#endif