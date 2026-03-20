#ifndef RAMFS_H
#define RAMFS_H

typedef unsigned int uint32_t;

#define RAMFS_MAX_FILES 32
#define RAMFS_MAX_NAME 32

void ramfs_init();
int ramfs_write(const char *name, const char *data, uint32_t len);
int ramfs_read(const char *name, char *buf, uint32_t buf_len);
int ramfs_exists(const char *name);
void ramfs_list(void (*callback)(const char *name, uint32_t size));

#endif