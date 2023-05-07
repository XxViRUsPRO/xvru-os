#include "vfs.h"
#include <x86.h>

i32 vfs_write(fd_t fd, const void *buf, i32 size)
{
    switch (fd)
    {
    case STDIN_FD:
        return 0;
    case STDOUT_FD:
    case STDERR_FD:
    case DEBUG_FD:
        for (i32 i = 0; i < size; i++)
            outb(0xE9, ((char *)buf)[i]);
        return size;
    default:
        return 0;
    }
}