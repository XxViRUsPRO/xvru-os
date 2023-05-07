#ifndef PMM_H
#define PMM_H
#include <types.h>

#define PMM_BLOCK_SIZE 4096
#define PMM_BLOCKS_PER_BYTE 8

static u32 pmm_used_blocks = 0;
static u32 pmm_max_blocks = 0;
static u32 *pmm_memory_map = 0;

void pmm_init(u32 start_address, u32 size);
void pmm_init_region(u32 base_address, u32 size);
void pmm_deinit_region(u32 base_address, u32 size);
u32 *pmm_alloc_blocks(u32 num_blocks);
void pmm_free_blocks(u32 *address, u32 num_blocks);
u32 pmm_get_max_blocks();
u32 pmm_get_used_blocks();
u32 pmm_get_free_blocks();

#endif