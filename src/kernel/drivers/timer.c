#include "timer.h"
#include <arch/i686/irq.h>
#include <x86.h>

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define PIT_MASK 0xFF
#define PIT_SET 0x36

#define PIT_HZ 1193180

static struct
{
    u64 frequency;
    u64 divisor;
    u64 ticks;
} state;

static void timer_set(int hz)
{
    outb(PIT_CONTROL, PIT_SET);

    u16 d = (u16)(PIT_HZ / hz);
    outb(PIT_A, d & PIT_MASK);
    outb(PIT_A, (d >> 8) & PIT_MASK);
}

u64 timer_get()
{
    // work around for the fact that we can't do floating point
    return state.ticks * 10;
}

static void timer_handler(Registers *regs)
{
    state.ticks++;
}

void timer_init()
{
    timer_set(TIMER_TPS);
    IRQ_install(0, timer_handler);
}

void sleep(u64 ms)
{
    u64 start = timer_get();
    while (timer_get() - start < ms)
        ;
}