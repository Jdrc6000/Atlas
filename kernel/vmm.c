#include "vmm.h"
#include "pmm.h"
#include "paging.h"

#define PD_INDEX(v)  ((v) >> 22)
#define PT_INDEX(v)  (((v) >> 12) & 0x3FF)
#define PAGE_ALIGN(v) ((v) & ~0xFFF)

void vmm_map(uint32_t *page_dir, uint32_t virt, uint32_t phys, uint32_t flags) {
    uint32_t pd_idx = PD_INDEX(virt);
    uint32_t pt_idx = PT_INDEX(virt);

    uint32_t *page_table;

    if (page_dir[pd_idx] & PAGE_PRESENT) {
        page_table = (uint32_t *)PAGE_ALIGN(page_dir[pd_idx]);
    } else {
        page_table = (uint32_t *)pmm_alloc();
        if (!page_table) return; // out of memory

        // zero it out
        for (int i = 0; i < 1024; i++)
            page_table[i] = 0;

        page_dir[pd_idx] = (uint32_t)page_table | PAGE_PRESENT | PAGE_WRITE | (flags & PAGE_USER);
    }

    page_table[pt_idx] = (phys & ~0xFFF) | PAGE_PRESENT | flags;
}

void vmm_unmap(uint32_t *page_dir, uint32_t virt) {
    uint32_t pd_idx = PD_INDEX(virt);
    uint32_t pt_idx = PT_INDEX(virt);

    if (!(page_dir[pd_idx] & PAGE_PRESENT)) return;

    uint32_t *page_table = (uint32_t *)PAGE_ALIGN(page_dir[pd_idx]);
    page_table[pt_idx] = 0;

    __asm__ volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

uint32_t vmm_get_phys(uint32_t *page_dir, uint32_t virt) {
    uint32_t pd_idx = PD_INDEX(virt);
    uint32_t pt_idx = PT_INDEX(virt);

    if (!(page_dir[pd_idx] & PAGE_PRESENT)) return 0;

    uint32_t *page_table = (uint32_t *)PAGE_ALIGN(page_dir[pd_idx]);
    if (!(page_table[pt_idx] & PAGE_PRESENT)) return 0;

    return PAGE_ALIGN(page_table[pt_idx]);
}