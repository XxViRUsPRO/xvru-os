#include "idt.h"

typedef struct
{
    u16 BaseLow;
    u16 Selector;
    u8 Reserved;
    u8 Flags;
    u16 BaseHigh;
} __attribute__((packed)) IDT_Entry;

typedef struct
{
    u16 Limit;
    IDT_Entry *Ptr;
} __attribute__((packed)) IDT_Descriptor;

void __attribute__((cdecl)) IDT_load(IDT_Descriptor *descriptor);

IDT_Entry idt[256];
IDT_Descriptor idt_descriptor = {
    sizeof(idt) - 1,
    idt};

void IDT_set_gate(u8 interrupt, void *base, u16 segment_descriptor, u8 flags)
{
    idt[interrupt].BaseLow = (u32)base & 0xFFFF;
    idt[interrupt].Selector = segment_descriptor;
    idt[interrupt].Reserved = 0;
    idt[interrupt].Flags = flags;
    idt[interrupt].BaseHigh = ((u32)base >> 16) & 0xFFFF;
}

void IDT_enable_gate(u8 interrupt)
{
    idt[interrupt].Flags |= IDT_FLAG_PRESENT;
}

void IDT_disable_gate(u8 interrupt)
{
    idt[interrupt].Flags &= ~IDT_FLAG_PRESENT;
}

void IDT_init(void)
{
    IDT_load(&idt_descriptor);
}