#ifndef GDT_H
#define GDT_H
#include <types.h>

#define GDT_CODE_SEG 0x08
#define GDT_DATA_SEG 0x10

void GDT_init(void);

#endif