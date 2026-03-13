#include "pmm.h"

#define TOTAL_PAGES (PMM_RAM / PAGE_SIZE)
#define BITMAP_SIZE (TOTAL_PAGES / 32)

static uint32_t bitmap[BITMAP_SIZE];

static void bitmap_set(uint32_t page) {
    bitmap[page / 32] |= (1 << (page % 32));
}

static void bitmap_clear(uint32_t page) {
    bitmap[page / 32] &= ~(1 << (page % 32));
}

static int bitmap_test(uint32_t page) {
    return bitmap[page / 32] & (1 << (page % 32));
}

void pmm_init() {
    // mark everything as used to start
    for (int i = 0; i < BITMAP_SIZE; i++)
        bitmap[i] = 0xFFFFFFFF;

    uint32_t free_start = 0x200000; // 2mb
    uint32_t free_end   = PMM_RAM;

    for (uint32_t addr = free_start; addr < free_end; addr += PAGE_SIZE)
        bitmap_clear(addr / PAGE_SIZE);
}

uint32_t pmm_alloc() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] == 0xFFFFFFFF) continue; // all used, skip
        for (int bit = 0; bit < 32; bit++) {
            if (!(bitmap[i] & (1 << bit))) {
                bitmap_set(i * 32 + bit);
                return (uint32_t)(i * 32 + bit) * PAGE_SIZE;
            }
        }
    }
    return 0; // out of memory
}

void pmm_free(uint32_t addr) {
    bitmap_clear(addr / PAGE_SIZE);
}