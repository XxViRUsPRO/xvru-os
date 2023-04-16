#include "paging.h"
#include <stdio.h>

extern void enablePaging(u32 *);

// paging mode :
// NON PAE 4K Mode (32 bit) - level 0 page, 1 PT, 2 PD
// NON PAE 4M Mode (32 bit) - level 0 page, 2 PD
// PAE 4K Mode (32 bit) - level 0 page, 1 PT, 2 PD, 3 PDP
// PAE 4M Mode (32 bit) - level 0 page, 2 PD, 3 PDP
// LONG MODE 4K (64 bit) - level 0 page, 1 PT, 2 PD, 3 PDP, 4 PML4

u32 page_directory[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));
u32 page_table[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));

void init_paging()
{
    i32 i;

    // Create page directory, supervisor mode, read/write, not present : 0 1 0 = 2
    for (i = 0; i < NUM_PAGES; i++)
    {
        page_directory[i] = 0x00000002;
        // page_directory[i] = (unsigned int)directory;
    }

    // Create page table, supervisor mode, read/write, present : 0 1 1 = 3
    // As the address is page aligned, it will always leave 12 bits zeroed.
    for (i = 0; i < NUM_PAGES; i++)
    {
        page_table[i] = (i * 0x1000) | 3;
    }

    // put page_table into page_directory supervisor level, read/write, present
    page_directory[0] = ((u32)page_table) | 3;

    enablePaging(page_directory);

    // register_interrupt_handler(14, handle_page_fault);
}

void page_fault()
{
    char message[] = "Page Fault";
    // serial_write(0x3F8, message, sizeof(message));
    printf(message);
}