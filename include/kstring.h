#ifndef KSTRING_H
#define KSTRING_H

typedef unsigned int uint32_t;

int kstrlen(const char *s);
int kstrcmp(const char *a, const char *b);
int kstrncmp(const char *a, const char *b, uint32_t n);
char *kstrcpy(char *dst, const char *src);
char *kstrncpy(char *dst, const char *src, uint32_t n);

void *kmemset(void *dst, int val, uint32_t n);
void *kmemcpy(void *dst, const void *src, uint32_t n);
int kmemcmp(const void *a, const void *b, uint32_t n);

#endif