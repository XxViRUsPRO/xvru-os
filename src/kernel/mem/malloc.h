#ifndef MALLOC_H
#define MALLOC_H
#include <types.h>

#define MALLOC_PAGE_SIZE 4096

typedef struct malloc_block
{
    u32 size;
    bool free;
    struct malloc_block *next;
} malloc_block_t;

malloc_block_t *malloc_head = NULL;
u32 malloc_virtual_address = NULL;
u32 malloc_physical_address = NULL;
u32 total_malloc_pages = 0;

void *malloc(u32 size);
void free(void *ptr);

#endif