#ifndef VMM_H
#define VMM_H
#include <types.h>

#define VMM_PAGE_SIZE 4096
#define VMM_PAGE_TABLE_SIZE 1024
#define VMM_PAGE_DIRECTORY_SIZE 1024

#define PTABLE_ADDR_SPACE_SIZE 0x400000
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

#define PD_INDEX(x) (((u32)(x) >> 22))
#define PF_INDEX(x) (((u32)(x) >> 12))
#define PT_INDEX(x) (PF_INDEX(x) & 0x3FF)

typedef struct
{
    bool present : 1;
    bool rw : 1;
    bool user : 1;
    bool w_through : 1;
    bool cache : 1;
    bool accessed : 1;
    bool dirty : 1;
    bool attribute_table : 1;
    bool global : 1;
    bool available : 3;
    u32 frame : 20;
} pt_entry_t;

typedef struct
{
    bool present : 1;
    bool rw : 1;
    bool user : 1;
    bool w_through : 1;
    bool cache : 1;
    bool accessed : 1;
    bool available : 1;
    bool page_size : 1;
    bool available2 : 4;
    u32 frame : 20;
} pd_entry_t;

typedef struct
{
    pt_entry_t entries[VMM_PAGE_TABLE_SIZE];
} PageTable;

typedef struct
{
    pd_entry_t entries[VMM_PAGE_DIRECTORY_SIZE];
} PageDirectory;

bool vmm_init();
void *vmm_alloc_page(pt_entry_t *e);
void vmm_free_page(pt_entry_t *e);
bool vmm_map_page(void *phys, void *virt);
void vmm_unmap_page(void *virt);
pt_entry_t *vmm_get_page(void *virt);
void vmm_flush_tlb(void *virt);
PageDirectory *vmm_get_pdirectory();
bool vmm_set_pdirectory(PageDirectory *pd);

#endif