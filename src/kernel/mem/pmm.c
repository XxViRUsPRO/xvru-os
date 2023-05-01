#include "pmm.h"
#include <string.h>
#include <debug.h>

void pmm_map_set(i32 bit)
{
    pmm_memory_map[bit / 32] |= (1 << (bit % 32));
}

void pmm_map_unset(i32 bit)
{
    pmm_memory_map[bit / 32] &= ~(1 << (bit % 32));
}

bool pmm_map_test(i32 bit)
{
    return pmm_memory_map[bit / 32] & (1 << (bit % 32));
}

u32 pmm_get_blocks_count()
{
    return pmm_max_blocks;
}

i32 pmm_first_free_block(u32 num_blocks)
{
    if (num_blocks == 0)
        return -1;

    for (u32 i = 0; i < pmm_max_blocks / 32; i++)
    {
        if (pmm_memory_map[i] != 0xFFFFFFFF)
        {
            for (i32 j = 0; j < 32; j++)
            {
                i32 bit = 1 << j;

                if (!(pmm_memory_map[i] & bit))
                {
                    i32 start_bit = i * 32 + bit;
                    u32 pmm_free_blocks = 0;

                    for (u32 count = 0; count <= num_blocks; count++)
                    {
                        if (!pmm_map_test(start_bit + count))
                            pmm_free_blocks++;

                        if (pmm_free_blocks == num_blocks)
                            return i * 32 + j;
                    }
                }
            }
        }
    }

    return -1;
}

void pmm_init(u32 start_address, u32 size)
{
    pmm_memory_map = (u32 *)start_address;
    pmm_max_blocks = size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_max_blocks;

    memset(pmm_memory_map, 0xFF, pmm_max_blocks / PMM_BLOCKS_PER_BYTE);
}

void pmm_init_region(u32 base_address, u32 size)
{
    i32 align = base_address / PMM_BLOCK_SIZE;
    i32 num_blocks = size / PMM_BLOCK_SIZE;

    for (; num_blocks > 0; num_blocks--)
    {
        pmm_map_unset(align++);
        pmm_used_blocks--;
    }

    pmm_map_set(0);
}

void pmm_deinit_region(u32 base_address, u32 size)
{
    i32 align = base_address / PMM_BLOCK_SIZE;
    i32 num_blocks = size / PMM_BLOCK_SIZE;

    for (; num_blocks > 0; num_blocks--)
    {
        pmm_map_set(align++);
        pmm_used_blocks++;
    }
}

u32 *pmm_alloc_blocks(u32 num_blocks)
{
    if ((pmm_max_blocks - pmm_used_blocks) <= num_blocks)
        return 0;

    i32 starting_block = pmm_first_free_block(num_blocks);
    if (starting_block == -1)
        return 0;

    for (u32 i = 0; i < num_blocks; i++)
        pmm_map_set(starting_block + i);

    pmm_used_blocks += num_blocks;

    u32 address = starting_block * PMM_BLOCK_SIZE;

    return (u32 *)address;
}

void pmm_free_blocks(u32 *address, u32 num_blocks)
{
    i32 starting_block = (u32)address / PMM_BLOCK_SIZE;

    for (u32 i = 0; i < num_blocks; i++)
        pmm_map_unset(starting_block + i);

    pmm_used_blocks -= num_blocks;
}
