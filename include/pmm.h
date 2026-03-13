#ifndef PMM_H
#define PMM_H

typedef unsigned int uint32_t;

#define PAGE_SIZE 4096
#define PMM_RAM 0x1000000

void pmm_init();
uint32_t pmm_alloc();
void pmm_free(uint32_t addr);

#endif