#ifndef VMM_H
#define VMM_H

#include "paging.h"

typedef unsigned int uint32_t;

void vmm_map(uint32_t *page_dir, uint32_t virt, uint32_t phys, uint32_t flags);
void vmm_unmap(uint32_t *page_dir, uint32_t virt);
uint32_t vmm_get_phys(uint32_t *page_dir, uint32_t virt);

#endif