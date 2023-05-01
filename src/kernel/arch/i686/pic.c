#include "pic.h"
#include <x86.h>

#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

enum
{
    PIC_ICW1_ICW4 = 0x01,
    PIC_ICW1_SINGLE = 0x02,
    PIC_ICW1_INTERVAL4 = 0x04,
    PIC_ICW1_LEVEL = 0x08,
    PIC_ICW1_INIT = 0x10,
} PIC_ICW1;

enum
{
    PIC_ICW4_8086 = 0x01,
    PIC_ICW4_AUTO_EOI = 0x02,
    PIC_ICW4_BUF_MASTER = 0x04,
    PIC_ICW4_BUF_SLAVE = 0x00,
    PIC_ICW4_BUFFERED = 0x08,
    PIC_ICW4_SFNM = 0x10,
} PIC_ICW4;

enum
{
    PIC_CMD_EOI = 0x20,
    PIC_CMD_READ_IRR = 0x0A,
    PIC_CMD_READ_ISR = 0x0B,
} PIC_CMD;

void PIC_config(u8 offsetPic1, u8 offsetPic2)
{
    // Initialize Control Word 1
    outb(PIC1_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();

    // Initialize Control Word 2 - offset
    outb(PIC1_DATA_PORT, offsetPic1);
    io_wait();
    outb(PIC2_DATA_PORT, offsetPic2);
    io_wait();

    // Initialize Control Word 3 - cascade
    outb(PIC1_DATA_PORT, 0x04); // this tells pic1 that there is a slave pic at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA_PORT, 0x02); // this tells pic2 its cascade identity (0000 0010)
    io_wait();

    // Initialize Control Word 4 - environment info
    outb(PIC1_DATA_PORT, PIC_ICW4_8086);
    io_wait();
    outb(PIC2_DATA_PORT, PIC_ICW4_8086);
    io_wait();

    // Clear data registers
    outb(PIC1_DATA_PORT, 0);
    io_wait();
    outb(PIC2_DATA_PORT, 0);
    io_wait();
}

void PIC_mask(u8 irq, bool set)
{
    u16 port;
    u8 value;

    if (irq < 8)
    {
        port = PIC1_DATA_PORT;
    }
    else
    {
        port = PIC2_DATA_PORT;
        irq -= 8;
    }
    value = inb(port);
    if (set)
    {
        value |= (1 << irq);
    }
    else
    {
        value &= ~(1 << irq);
    }
    outb(port, value);
}

void PIC_send_EOI(u8 irq)
{
    if (irq >= 8)
    {
        outb(PIC2_CMD_PORT, PIC_CMD_EOI);
    }
    outb(PIC1_CMD_PORT, PIC_CMD_EOI);
}

u16 PIC_get_irr_reg()
{
    outb(PIC1_CMD_PORT, PIC_CMD_READ_IRR);
    outb(PIC2_CMD_PORT, PIC_CMD_READ_IRR);
    return inb(PIC1_CMD_PORT) | (inb(PIC2_CMD_PORT) << 8);
}

u16 PIC_get_isr_reg()
{
    outb(PIC1_CMD_PORT, PIC_CMD_READ_ISR);
    outb(PIC2_CMD_PORT, PIC_CMD_READ_ISR);
    return inb(PIC1_CMD_PORT) | (inb(PIC2_CMD_PORT) << 8);
}

void PIC_disable()
{
    outb(PIC1_DATA_PORT, 0xFF);
    io_wait();
    outb(PIC2_DATA_PORT, 0xFF);
    io_wait();
}
