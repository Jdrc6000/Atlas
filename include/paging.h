#ifndef PAGING_H
#define PAGING_H

typedef unsigned int uint32_t;

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE (1 << 1)
#define PAGE_USER (1 << 2)

void paging_init();
uint32_t *paging_get_directory();

#endif