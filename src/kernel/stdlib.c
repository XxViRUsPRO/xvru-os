#include "stdlib.h"
#include <mem/malloc.h>

void *malloc(u32 size)
{
    if (size == 0)
        return 0;

    if (!malloc_get_list_head())
        malloc_init(size);

    void *ptr = malloc_next_block(size);

    merge_free_blocks();

    return ptr;
}

void free(void *ptr)
{
    malloc_free(ptr);
}