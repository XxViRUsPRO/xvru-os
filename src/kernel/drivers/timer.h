#ifndef TIMER_H
#define TIMER_H
#include <types.h>

#define TIMER_TPS 100

u64 timer_get();
void timer_init();
void sleep(u64 ms);

#endif