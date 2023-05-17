#include "vmm.h"
#include "pmm.h"
#include <string.h>
#include <memdefs.h>

PageDirectory *vmm_current_directory = NULL;

pt_entry_t *ptable_lookup_entry(PageTable *p, u32 addr)
{
    if (p)
        return &p->entries[PT_INDEX(addr)];
    return 0;
}

pd_entry_t *pdirectory_lookup_entry(PageDirectory *p, u32 addr)
{
    if (p)
        return &p->entries[PT_INDEX(addr)];
    return 0;
}

bool vmm_set_pdirectory(PageDirectory *pd)
{
    if (!pd)
        return false;

    vmm_current_directory = pd;
    __asm__ __volatile__("movl %%eax, %%cr3" ::"a"(vmm_current_directory));
    return true;
}

PageDirectory *vmm_get_pdirectory()
{
    return vmm_current_directory;
}

void vmm_flush_tlb(void *virt)
{
    __asm__ __volatile__("cli; invlpg (%0); sti" ::"r"(virt));
}

bool vmm_init()
{
    // create default page directory
    PageDirectory *directory = (PageDirectory *)pmm_alloc_blocks(3);
    if (!directory)
        return false;

    // clear page directory
    memset(directory, 0, sizeof(PageDirectory));
    for (i32 i = 0; i < 1024; i++)
        directory->entries[i].rw = 1;

    // allocate default page table (0 - 4MB)
    PageTable *table = (PageTable *)pmm_alloc_blocks(1);
    if (!table)
        return false;

    // allocate 3GB page table
    PageTable *table3gb = (PageTable *)pmm_alloc_blocks(1);
    if (!table3gb)
        return false;

    // clear page table
    memset(table, 0, sizeof(PageTable));
    memset(table3gb, 0, sizeof(PageTable));

    // 1st 4MB are idenitity mapped
    for (i32 i = 0, frame = 0x0, virt = 0x0; i < 1024; i++, frame += VMM_PAGE_SIZE, virt += VMM_PAGE_SIZE)
    {
        // create a new page
        pt_entry_t page = {0};
        page.present = 1;
        page.rw = 1;
        page.frame = ((u32)frame) >> 12;

        // ...and add it to the page table
        table3gb->entries[PT_INDEX(virt)] = page;
    }

    // map 1mb to 3gb
    for (i32 i = 0, frame = MEMORY_KERNEL_ADDR, virt = 0xC0000000; i < 1024; i++, frame += VMM_PAGE_SIZE, virt += VMM_PAGE_SIZE)
    {
        // create a new page
        pt_entry_t page = {0};
        page.present = 1;
        page.rw = 1;
        page.frame = ((u32)frame) >> 12;

        // ...and add it to the page table
        table->entries[PT_INDEX(virt)] = page;
    }

    pd_entry_t *entry = &directory->entries[PD_INDEX(0xC0000000)];
    entry->present = 1;
    entry->rw = 1;
    entry->frame = ((u32)table) >> 12;

    pd_entry_t *entry3gb = &directory->entries[PD_INDEX(0x00000000)];
    entry3gb->present = 1;
    entry3gb->rw = 1;
    entry3gb->frame = ((u32)table3gb) >> 12;

    // switch to our page directory
    vmm_set_pdirectory(directory);

    // enable paging
    __asm__ __volatile__("movl %cr0, %eax; orl $0x80000001, %eax; movl %eax, %cr0");

    return true;
}

void *vmm_alloc_page(pt_entry_t *e)
{
    void *p = pmm_alloc_blocks(1);
    if (p)
    {
        e->present = 1;
        e->frame = (u32)p;
    }
    return p;
}

void vmm_free_page(pt_entry_t *e)
{
    void *p = (void *)PF_INDEX(e);
    if (p)
        pmm_free_blocks(p, 1);
    e->present = 0;
}

bool vmm_map_page(void *phys, void *virt)
{
    PageDirectory *pageDirectory = vmm_get_pdirectory();
    pd_entry_t *pde = &pageDirectory->entries[PD_INDEX((u32)virt)];
    if (!pde->present)
    {
        PageTable *table = (PageTable *)pmm_alloc_blocks(1);
        if (!table)
            return false;

        memset(table, 0, sizeof(PageTable));
        pd_entry_t *entry = &pageDirectory->entries[PD_INDEX((u32)virt)];

        entry->present = 1;
        entry->rw = 1;
        entry->frame = ((u32)table) >> 12;
    }
    PageTable *table = (PageTable *)PF_INDEX(pde);

    pt_entry_t *entry = &table->entries[PT_INDEX((u32)virt)];

    entry->present = 1;
    entry->frame = ((u32)phys) >> 12;

    return true;
}

void vmm_unmap_page(void *virt)
{
    pt_entry_t *entry = vmm_get_page(virt);

    entry->present = 0;
    entry->frame = 0;
}

pt_entry_t *vmm_get_page(void *virt)
{
    PageDirectory *pageDirectory = vmm_current_directory;
    pd_entry_t *pde = &pageDirectory->entries[PD_INDEX((u32)virt)];
    PageTable *table = (PageTable *)PF_INDEX(pde);
    pt_entry_t *entry = &table->entries[PT_INDEX((u32)virt)];
    return entry;
}
