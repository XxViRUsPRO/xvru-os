#include <stdlib.h>
#include <stdio.h>
#include <hal/hal.h>
#include <types.h>
#include <system.h>
#include <math.h>
#include <x86.h>
#include <drivers/timer.h>
#include <display.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <mem/malloc.h>

void __attribute__((section(".entry"))) kmain(KernelArgs *kernelArgs)
{
    // Initialize the HAL
    HAL_init();
    timer_init();

    // Use MemoryManagerParams to set up the PMM and VMM
    MemoryManagerParams *mmParams = &kernelArgs->mmParams;
    pmm_set_memory_map((u32 *)mmParams->memoryMap);
    pmm_set_max_blocks(mmParams->maxBlocks);
    pmm_set_used_blocks(mmParams->usedBlocks);
    if (!vmm_set_pdirectory((PageDirectory *)mmParams->pageDirectory))
    {
        PANIC("Failed to set page directory");
    }

    // Set up intial Malloc list head
    // u32 mallocVirtAddr = (u32)kernelArgs->kernelMemoryInfo.kernelEnd;
    u32 mallocVirtAddr = 0x300000;
    u32 mallocPhysAddr = (u32)pmm_alloc_blocks(1);
    u32 totalMallocPages = 1;

    vmm_map_page((void *)mallocPhysAddr, (void *)mallocVirtAddr);
    pt_entry_t *page = vmm_get_page((void *)mallocVirtAddr);
    page->rw = 1;

    malloc_block_t *mallocListHead = (malloc_block_t *)mallocVirtAddr;
    mallocListHead->size = (totalMallocPages * MALLOC_PAGE_SIZE) - sizeof(malloc_block_t);
    mallocListHead->free = true;
    mallocListHead->next = 0;

    malloc_set_params(mallocListHead, mallocVirtAddr, mallocPhysAddr, totalMallocPages);

    // Initialize the display
    // Map the framebuffer region
    u32 *fb = (u32 *)0xfd000000;
    const u32 fb_size = 800 * 600 * (32 / 8);
    u32 fb_size_pages = ceil((double)fb_size / 4096);
    fb_size_pages *= 2;
    for (u32 i = 0, fb_start = (u32)fb; i < fb_size_pages; i++, fb_start += 4096)
    {
        vmm_map_page((void *)fb_start, (void *)fb_start);
    }

    // Deinit the framebuffer region
    pmm_deinit_region((u32)fb, fb_size_pages * 4096);

    display_init(fb, 800, 600, 800, 32);
    display_clear(COLOR(255, 255, 0));
    display_render();

    // Infinite loop to prevent the kernel from exiting
    while (1)
        ;

    return;
}
