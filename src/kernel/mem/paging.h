#ifndef PAGING_H
#define PAGING_H
#include <types.h>

#define NUM_PAGES 1024
#define PAGE_FRAME_SIZE 4096

#define PAGE_PRESENT 1
#define PAGE_READONLY 0
#define PAGE_READWRITE 1
#define PAGE_USER 1
#define PAGE_SUPERVISOR 0
#define PAGE_SIZE_4KB 0
#define PAGE_SIZE_4MB 1

typedef struct page
{
    u32 present : 1;  // Page present in memory
    u32 rw : 1;       // Read-only if clear, readwrite if set
    u32 user : 1;     // Supervisor level only if clear
    u32 accessed : 1; // Has the page been accessed since last refresh?
    u32 dirty : 1;    // Has the page been written to since last refresh?
    u32 unused : 7;   // Amalgamation of unused and reserved bits
    u32 frame : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[NUM_PAGES] __attribute__((aligned(PAGE_FRAME_SIZE)));
} page_table_t;

typedef struct page_directory
{
    /**
       Array of pointers to pagetables.
    **/
    page_table_t *tables[1024];
    /**
       Array of pointers to the pagetables above, but gives their *physical*
       location, for loading into the CR3 register.
    **/
    u32 tablesPhysical[1024];
    /**
       The physical address of tablesPhysical. This comes into play
       when we get our kernel heap allocated and the directory
       may be in a different location in virtual memory.
    **/
    u32 physicalAddr;
} page_directory_t;

/**
  Sets up the environment, page directories etc and
  enables paging.
**/
void paging_init();

void page_fault();

#endif