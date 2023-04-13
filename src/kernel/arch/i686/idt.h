#ifndef IDT_H
#define IDT_H
#include <types.h>

typedef enum
{
    IDT_FLAG_GATE_TASK = 0x05,
    IDT_FLAG_GATE_16INT = 0x06,
    IDT_FLAG_GATE_16TRAP = 0x07,
    IDT_FLAG_GATE_32INT = 0x0E,
    IDT_FLAG_GATE_32TRAP = 0x0F,

    IDT_FLAG_RING0 = (0x00 << 5),
    IDT_FLAG_RING1 = (0x01 << 5),
    IDT_FLAG_RING2 = (0x02 << 5),
    IDT_FLAG_RING3 = (0x03 << 5),

    IDT_FLAG_PRESENT = 0x80,
} IDT_FLAGS;

void IDT_init(void);
void IDT_set_gate(u8 interrupt, void *base, u16 segment_descriptor, u8 flags);
void IDT_enable_gate(u8 interrupt);
void IDT_disable_gate(u8 interrupt);

#endif