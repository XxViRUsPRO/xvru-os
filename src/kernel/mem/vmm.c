#include "vmm.h"
#include "pmm.h"
#include <x86.h>
#include <string.h>

const u32 KERNEL_ADDR = 0x100000;

PageDirectory *vmm_current_directory = 0;

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

bool vmm_set_pdirectory(PageDirectory *p)
{
    if (!p)
        return false;

    vmm_current_directory = p;
    x86_load_pdbr(p);
    return true;
}

void vmm_flush_tlb(void *virt)
{
    x86_flush_tlb(virt);
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
        directory->entries[i] = PTE_WRITABLE;

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
        pt_entry_t page = 0;
        SET_ATTRIB(&page, PTE_PRESENT);
        SET_ATTRIB(&page, PTE_WRITABLE);
        SET_FRAME(&page, frame);

        // ...and add it to the page table
        table3gb->entries[PT_INDEX(virt)] = page;
    }

    // map 1mb to 3gb
    for (i32 i = 0, frame = KERNEL_ADDR, virt = 0xC0000000; i < 1024; i++, frame += VMM_PAGE_SIZE, virt += VMM_PAGE_SIZE)
    {
        // create a new page
        pt_entry_t page = 0;
        SET_ATTRIB(&page, PTE_PRESENT);
        SET_ATTRIB(&page, PTE_WRITABLE);
        SET_FRAME(&page, frame);

        // ...and add it to the page table
        table->entries[PT_INDEX(virt)] = page;
    }

    pd_entry_t *entry = &directory->entries[PD_INDEX(0xc0000000)];
    SET_ATTRIB(entry, PDE_PRESENT);
    SET_ATTRIB(entry, PDE_WRITABLE);
    SET_FRAME(entry, (u32)table);

    pd_entry_t *entry3gb = &directory->entries[PD_INDEX(0x00000000)];
    SET_ATTRIB(entry3gb, PDE_PRESENT);
    SET_ATTRIB(entry3gb, PDE_WRITABLE);
    SET_FRAME(entry3gb, (u32)table3gb);

    // switch to our page directory
    vmm_set_pdirectory(directory);

    // enable paging
    x86_enable_paging();

    return true;
}

void *vmm_alloc_page(pt_entry_t *e)
{
    void *p = pmm_alloc_blocks(1);
    if (p)
    {
        SET_FRAME(e, (u32)p);
        SET_ATTRIB(e, PTE_PRESENT);
    }
    return p;
}

void vmm_free_page(pt_entry_t *e)
{
    void *p = (void *)PAGE_GET_PHYSICAL_ADDRESS(e);
    if (p)
        pmm_free_blocks(p, 1);
    CLEAR_ATTRIB(e, PTE_PRESENT);
}

bool vmm_map_page(void *phys, void *virt)
{
    PageDirectory *pageDirectory = vmm_current_directory;
    pd_entry_t *pde = &pageDirectory->entries[PD_INDEX((u32)virt)];
    if ((*pde & PTE_PRESENT) != PTE_PRESENT)
    {
        PageTable *table = (PageTable *)pmm_alloc_blocks(1);
        if (!table)
            return false;

        memset(table, 0, sizeof(PageTable));

        pd_entry_t *entry = &pageDirectory->entries[PD_INDEX((u32)virt)];

        SET_ATTRIB(entry, PDE_PRESENT);
        SET_ATTRIB(entry, PDE_WRITABLE);
        SET_FRAME(entry, (u32)table);
    }

    PageTable *table = (PageTable *)PAGE_GET_PHYSICAL_ADDRESS(pde);

    pt_entry_t *entry = &table->entries[PT_INDEX((u32)virt)];

    SET_ATTRIB(entry, PTE_PRESENT);
    SET_FRAME(entry, (u32)phys);

    return true;
}

void vmm_unmap_page(void *virt)
{
    pt_entry_t *entry = vmm_get_page(virt);

    SET_FRAME(entry, 0x0);
    CLEAR_ATTRIB(entry, PTE_PRESENT);
}

pt_entry_t *vmm_get_page(void *virt)
{
    PageDirectory *pageDirectory = vmm_current_directory;
    pd_entry_t *pde = &pageDirectory->entries[PD_INDEX((u32)virt)];
    PageTable *table = (PageTable *)PAGE_GET_PHYSICAL_ADDRESS(pde);
    pt_entry_t *entry = &table->entries[PT_INDEX((u32)virt)];
    return entry;
}
