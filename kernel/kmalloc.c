#include "kmalloc.h"

#define HEAP_MAX 0x100000 // 1mb
#define ALIGN4(n) (((n) + 3) & ~3)

typedef struct block_header {
    uint32_t size; // usable bytes
    uint32_t free;
    struct block_header *next; // next block (linked list)
} block_header_t;

#define HEADER_SIZE ALIGN4(sizeof(block_header_t))

extern char heap_start;
static char *heap_base = 0;
static char *heap_ptr = 0;
static block_header_t *first = 0;

void kmalloc_init() {
    heap_base = &heap_start;
    heap_ptr  = &heap_start;
    first     = 0;
}

// my new fav word
static void coalesce(block_header_t *b) {
    while (b->next && b->next->free) {
        b->size += HEADER_SIZE + b->next->size;
        b->next  = b->next->next;
    }
}

void *kmalloc(uint32_t size) {
    if (!size) return 0;
    size = ALIGN4(size);

    /* walk block list, first-fit */
    block_header_t *b = first;
    while (b) {
        if (b->free && b->size >= size) {
            /* optionally split if leftover is big enough */
            if (b->size >= size + HEADER_SIZE + 4) {
                block_header_t *split =
                    (block_header_t *)((char *)b + HEADER_SIZE + size);
                split->size = b->size - HEADER_SIZE - size;
                split->free = 1;
                split->next = b->next;
                b->next     = split;
                b->size     = size;
            }
            b->free = 0;
            return (char *)b + HEADER_SIZE;
        }
        b = b->next;
    }

    /* no fitting free block — bump allocate a new one */
    if ((uint32_t)(heap_ptr + HEADER_SIZE + size) >
        (uint32_t)(heap_base + HEAP_MAX))
        return 0;   /* OOM */

    b        = (block_header_t *)heap_ptr;
    b->size  = size;
    b->free  = 0;
    b->next  = 0;
    heap_ptr += HEADER_SIZE + size;

    /* append to block list */
    if (!first) {
        first = b;
    } else {
        block_header_t *tail = first;
        while (tail->next) tail = tail->next;
        tail->next = b;
    }

    return (char *)b + HEADER_SIZE;
}

void kfree(void *ptr) {
    if (!ptr) return;
    block_header_t *b = (block_header_t *)((char *)ptr - HEADER_SIZE);
    b->free = 1;
    coalesce(b);
}

uint32_t kmalloc_used() {
    uint32_t used = 0;
    block_header_t *b = first;
    while (b) {
        if (!b->free) used += b->size;
        b = b->next;
    }
    return used;
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