#ifndef _GDT_H_
#define _GDT_H_
#include <types.h>

#define GDT_CODE_SEG 0x08
#define GDT_DATA_SEG 0x10

void GDT_init(void);

#endif