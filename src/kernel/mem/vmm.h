#ifndef VMM_H
#define VMM_H
#include <types.h>

#define PAGE_SIZE 4096
#define PAGE_TABLE_SIZE 1024
#define PAGE_DIRECTORY_SIZE 1024

#define PTABLE_ADDR_SPACE_SIZE 0x400000
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

#define PD_INDEX(x) (((x) >> 22))
#define PT_INDEX(x) (((x) >> 12) & 0x3FF)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xFFF)

#define SET_FRAME(x, addr) (*x = (*x & ~0x7FFFF000) | addr)
#define SET_ATTRIB(x, attrib) (*x |= attrib)
#define CLEAR_ATTRIB(x, attrib) (*x &= ~attrib)

typedef u32 pt_entry_t;
typedef u32 pd_entry_t;

enum EPageTableEntryFlags
{
    PTE_PRESENT = 0x1,
    PTE_WRITABLE = 0x2,
    PTE_USER = 0x4,
    PTE_WRITETHROUGH = 0x8,
    PTE_NOT_CACHEABLE = 0x10,
    PTE_ACCESSED = 0x20,
    PTE_DIRTY = 0x40,
    PTE_PAT = 0x80,
    PTE_CPU_GLOBAL = 0x100,
    PTE_LV4_GLOBAL = 0x200,
    PTE_FRAME = 0x7FFFF000,
};

enum EPageDirectoryEntryFlags
{
    PDE_PRESENT = 0x1,
    PDE_WRITABLE = 0x2,
    PDE_USER = 0x4,
    PDE_WRITETHROUGH = 0x8,
    PDE_NOT_CACHEABLE = 0x10,
    PDE_ACCESSED = 0x20,
    PDE_4MB = 0x80,
    PDE_CPU_GLOBAL = 0x100,
    PDE_PAT = 0x2000,
    PDE_FRAME = 0x7FFFF000,
};

typedef struct
{
    pt_entry_t entries[PAGE_TABLE_SIZE];
} PageTable;

typedef struct
{
    pd_entry_t entries[PAGE_DIRECTORY_SIZE];
} PageDirectory;

bool vmm_init();
bool vmm_alloc_page(pt_entry_t *e);
void vmm_free_page(pt_entry_t *e);

#endif