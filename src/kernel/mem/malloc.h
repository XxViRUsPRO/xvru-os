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

void malloc_init(u32 bytes);
void malloc_split(malloc_block_t *node, u32 size);
void *malloc_next_block(u32 size);
void merge_free_blocks(void);
void malloc_free(void *ptr);
malloc_block_t *malloc_get_list_head(void);

#endif