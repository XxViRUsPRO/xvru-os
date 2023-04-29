#include "vesa.h"
#include "x86.h"

bool vbe_get_info(VbeInfoBlock *info)
{
    return 0x004f == x86_vbe_get_info_(info);
}

bool vbe_get_mode_info(u16 mode, VbeModeInfoBlock *info)
{
    return 0x004f == x86_vbe_get_mode_info_(mode, info);
}

bool vbe_set_mode(u16 mode)
{
    return 0x004f == x86_vbe_set_mode_(mode);
}

bool vbe_get_mode(u16 *mode)
{
    return 0x004f == x86_vbe_get_mode_(mode);
}