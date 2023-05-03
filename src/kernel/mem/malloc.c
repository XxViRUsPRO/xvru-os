#include "malloc.h"
#include "vmm.h"
#include "pmm.h"

malloc_block_t *malloc_list_head = 0;
u32 malloc_virt_address = 0;
u32 malloc_phys_address = 0;
u32 total_malloc_pages = 0;

malloc_block_t *malloc_get_list_head()
{
    return malloc_list_head;
}

void malloc_init(u32 bytes)
{
    total_malloc_pages = bytes / MALLOC_PAGE_SIZE;
    if (bytes % MALLOC_PAGE_SIZE > 0)
        total_malloc_pages++;

    malloc_phys_address = (u32)pmm_alloc_blocks(total_malloc_pages);
    malloc_list_head = (malloc_block_t *)malloc_virt_address;

    for (u32 i = 0, virt = malloc_virt_address; i < total_malloc_pages; i++, virt += MALLOC_PAGE_SIZE)
    {
        vmm_map_page((void *)(malloc_phys_address + i * MALLOC_PAGE_SIZE), (void *)virt);

        pt_entry_t *page = vmm_get_page((void *)virt);

        SET_ATTRIB(page, PTE_WRITABLE);
    }

    if (malloc_list_head)
    {
        malloc_list_head->size = (total_malloc_pages * MALLOC_PAGE_SIZE) - sizeof(malloc_block_t);
        malloc_list_head->free = true;
        malloc_list_head->next = 0;
    }
}

void malloc_split(malloc_block_t *node, u32 size)
{
    malloc_block_t *new_node = (malloc_block_t *)((void *)node + size + sizeof(malloc_block_t));

    new_node->size = node->size - size - sizeof(malloc_block_t);
    new_node->free = true;
    new_node->next = node->next;

    node->size = size;
    node->free = false;
    node->next = new_node;
}

void *malloc_next_block(u32 size)
{
    malloc_block_t *cur = 0, *prev = 0;

    if (size == 0)
        return 0;

    if (!malloc_list_head->size)
        malloc_init(size);

    cur = malloc_list_head;
    while (((cur->size < size) || !cur->free) && cur->next)
    {
        prev = cur;
        cur = cur->next;
    }

    if (size == cur->size)
    {
        cur->free = false;
    }
    else if (cur->size > size + sizeof(malloc_block_t))
    {
        malloc_split(cur, size);
    }
    else
    {
        u8 num_pages = 1;
        while (cur->size + num_pages * MALLOC_PAGE_SIZE < size + sizeof(malloc_block_t))
            num_pages++;

        u32 virt = malloc_virt_address + total_malloc_pages * MALLOC_PAGE_SIZE;

        for (u8 i = 0; i < num_pages; i++)
        {
            pt_entry_t page = 0;
            u32 *temp = vmm_alloc_page(&page);

            vmm_map_page((void *)temp, (void *)virt);
            SET_ATTRIB(&page, PTE_WRITABLE);
            virt += MALLOC_PAGE_SIZE;
            cur->size += MALLOC_PAGE_SIZE;
            total_malloc_pages++;
        }

        malloc_split(cur, size);
    }

    return (void *)cur + sizeof(malloc_block_t);
}

void merge_free_blocks(void)
{
    malloc_block_t *cur = malloc_list_head, *prev = 0;

    while (cur && cur->next)
    {
        if (cur->free && cur->next->free)
        {
            cur->size += (cur->next->size) + sizeof(malloc_block_t);
            cur->next = cur->next->next;
        }
        prev = cur;
        cur = cur->next;
    }
}

void malloc_free(void *ptr)
{
    for (malloc_block_t *cur = malloc_list_head; cur->next; cur = cur->next)
        if ((void *)cur + sizeof(malloc_block_t) == ptr)
        {
            cur->free = true;
            merge_free_blocks();
            break;
        }
}
