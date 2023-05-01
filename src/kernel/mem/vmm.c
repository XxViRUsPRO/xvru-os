#include "vmm.h"
#include "pmm.h"
#include <x86.h>
#include <string.h>

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
        return &p->entries[PD_INDEX(addr)];
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

bool vmm_init()
{
    // allocate default page table
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
    for (int i = 0, frame = 0x0, virt = 0x0; i < 1024; i++, frame += 4096, virt += 4096)
    {
        // create a new page
        pt_entry_t page = 0;
        SET_ATTRIB(&page, PTE_PRESENT);
        SET_FRAME(&page, frame);

        // ...and add it to the page table
        table3gb->entries[PT_INDEX(virt)] = page;
    }

    // map 1mb to 3gb
    for (int i = 0, frame = 0x100000, virt = 0xc0000000; i < 1024; i++, frame += 4096, virt += 4096)
    {
        // create a new page
        pt_entry_t page = 0;
        SET_ATTRIB(&page, PTE_PRESENT);
        SET_FRAME(&page, frame);

        // ...and add it to the page table
        table->entries[PT_INDEX(virt)] = page;
    }

    // create default page directory
    PageDirectory *directory = (PageDirectory *)pmm_alloc_blocks(3);
    if (!directory)
        return false;

    // clear page directory and add page table
    memset(directory, 0, sizeof(PageDirectory));

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

bool vmm_alloc_page(pt_entry_t *e)
{
    void *p = pmm_alloc_blocks(1);
    if (!p)
        return false;

    SET_FRAME(e, (u32)p);
    SET_ATTRIB(e, PTE_PRESENT);
    return true;
}

void vmm_free_page(pt_entry_t *e)
{
    void *p = (void *)PAGE_GET_PHYSICAL_ADDRESS(e);
    if (p)
        pmm_free_blocks(p, 1);
    CLEAR_ATTRIB(e, PTE_PRESENT);
}

PageDirectory *vmm_get_pdirectory()
{
    return vmm_current_directory;
}

void vmm_map_page(void *phys, void *virt)
{
    PageDirectory *pageDirectory = vmm_get_pdirectory();
    pd_entry_t *pde = pdirectory_lookup_entry(pageDirectory, (u32)virt);
    if ((*pde & PDE_PRESENT) != PDE_PRESENT)
    {
        PageTable *table = (PageTable *)pmm_alloc_blocks(1);
        if (!table)
            return;
        memset(table, 0, sizeof(PageTable));
        pd_entry_t *entry = pdirectory_lookup_entry(pageDirectory, (u32)virt);
        SET_ATTRIB(entry, PDE_PRESENT);
        SET_ATTRIB(entry, PDE_WRITABLE);
        SET_FRAME(entry, (u32)table);
    }

    PageTable *table = (PageTable *)PAGE_GET_PHYSICAL_ADDRESS(pde);
    pt_entry_t *entry = ptable_lookup_entry(table, (u32)virt);
    SET_FRAME(entry, (u32)phys);
    SET_ATTRIB(entry, PTE_PRESENT);
}
