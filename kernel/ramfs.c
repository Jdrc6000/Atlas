#include "ramfs.h"
#include "kmalloc.h"
#include "kstring.h"

typedef struct {
    char name[RAMFS_MAX_NAME];
    char *data;
    uint32_t size;
    int used;
} ramfs_entry_t;

static ramfs_entry_t files[RAMFS_MAX_FILES];

void ramfs_init() {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        files[i].used = 0;
        files[i].data = 0;
        files[i].size = 0;
    }
}

static ramfs_entry_t *find(const char *name) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++)
        if (files[i].used && kstrcmp(files[i].name, name))
            return &files[i];
    return 0;
}

int ramfs_write(const char *name, const char *data, uint32_t len) {
    ramfs_entry_t *e = find(name);

    if (!e) {
        // find a free slot
        for (int i = 0; i < RAMFS_MAX_FILES; i++) {
            if (!files[i].used) {
                e = &files[i];
                break;
            }
        }

        if (!e) return -1; // no free slots
        e->used = 1;
        e->data = 0;
        e->size = 0;
        kstrncpy(e->name, name, RAMFS_MAX_NAME - 1);
        e->name[RAMFS_MAX_NAME - 1] = '\0';
    }

    if (e->data) kfree(e->data);
    e->data = kmalloc(len + 1);
    if (!e->data) return -1;
    kmemcpy(e->data, data, len);
    e->data[len] = '\0';
    e->size = len;
    return 0;
}

int ramfs_read(const char *name, char *buf, uint32_t buf_len) {
    ramfs_entry_t *e = find(name);
    if (!e) return -1;
    uint32_t copy = e->size < buf_len - 1 ? e->size : buf_len - 1;
    kmemcpy(buf, e->data, copy);
    buf[copy] = '\0';
    return (int)copy;
}

int ramfs_exists(const char *name) {
    return find(name) != 0;
}

void ramfs_list(void (*callback)(const char *name, uint32_t size)) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++)
        if (files[i].used)
            callback(files[i].name, files[i].size);
}