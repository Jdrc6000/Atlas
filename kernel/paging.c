#include "paging.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t page_table_0[1024] __attribute__((aligned(4096)));

extern void paging_enable(uint32_t *page_directory);

void paging_init() {
    for (int i = 0; i < 1024; i++) {
        uint32_t phys = i * 4096;
        page_table_0[i] = phys | PAGE_PRESENT | PAGE_WRITE;
    }

    page_directory[0] = (uint32_t)page_table_0 | PAGE_PRESENT | PAGE_WRITE;

    for (int i = 1; i < 1024; i++)
        page_directory[i] = 0;
    
    paging_enable(page_directory);
}