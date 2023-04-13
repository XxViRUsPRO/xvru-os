#ifndef ISR_H
#define ISR_H
#include <types.h>

typedef struct
{
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} __attribute__((packed)) Registers;

typedef void (*ISR_handler_t)(Registers *regs);

void ISR_init();
void ISR_install(u8 int_no, ISR_handler_t handler);

#endif