#ifndef ASM_H
#define ASM_H
#include <types.h>
#define _cdecl __attribute__((cdecl))

_cdecl void outb(u16 port, u8 value);
_cdecl u8 inb(u16 port);
_cdecl bool x86_disk_get_params_(u8 drive, u8 *drive_type, u16 *cylinders, u16 *sectors, u16 *heads);
_cdecl bool x86_disk_read_(u8 drive, u16 cylinder, u16 sector, u16 head, u8 count, void *buffer);
_cdecl bool x86_disk_reset_(u8 drive);

// VESA/VBE
_cdecl u16 x86_vbe_get_info_(void *info);
_cdecl u16 x86_vbe_get_mode_info_(u16 mode, void *info);
_cdecl u16 x86_vbe_set_mode_(u16 mode);
_cdecl u16 x86_vbe_get_mode_(u16 *mode);

#endif