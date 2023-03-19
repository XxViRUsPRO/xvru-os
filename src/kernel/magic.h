#ifndef _MAGIC_H_
#define _MAGIC_H_
#include "types.h"

#define CDECL __attribute__((cdecl))
#define PACKED __attribute__((packed))

void CDECL outb(u16 port, u8 value);
u8 CDECL inb(u16 port);

#endif