#ifndef _ISR_H_
#define _ISR_H_
#include <types.h>

typedef struct
{
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} __attribute__((packed)) Registers;

void ISR_init();

#endif