#include "kmalloc.h"

#define HEAP_MAX 0x100000 // 1mb

extern char heap_start;

static char *heap_ptr = 0;
static char *heap_base = 0;

void kmalloc_init() {
    heap_base = &heap_start;
    heap_ptr = &heap_start;
}

void *kmalloc(uint32_t size) {
    if (!size)
        return 0;

    // align to 4 bytes
    size = (size + 3) & ~3;

    // check we haven't overflowed
    if ((uint32_t)(heap_ptr + size) > (uint32_t)(heap_base + HEAP_MAX))
        return 0; // out of memory

    void *ptr = heap_ptr;
    heap_ptr += size;
    return ptr;
}

uint32_t kmalloc_used() {
    return (uint32_t)(heap_ptr - heap_base);
}

char *kstrdup(const char *s) {
    uint32_t len = 0;
    while (s[len]) len++;
    char *copy = kmalloc(len + 1);
    if (!copy) return 0;
    for (uint32_t i = 0; i <= len; i++)
        copy[i] = s[i];
    return copy;
}