#ifndef KMALLOC_H
#define KMALLOC_H

typedef unsigned int uint32_t;

void kmalloc_init();
void *kmalloc(uint32_t size);
void kfree(void *ptr);
uint32_t kmalloc_used();
char *kstrdup(const char *s);

#endif