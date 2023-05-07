#ifndef VFS_H
#define VFS_H
#include <types.h>

typedef i32 fd_t;

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2
#define DEBUG_FD 3

i32 vfs_write(fd_t fd, const void *buf, i32 size);

#endif