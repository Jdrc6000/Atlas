#include "vga.h"
#include "ramfs.h"
#include "kmalloc.h"
#include "kstring.h"

#define WRITE_BUF 1024

int cmd_write(int argc, char **argv) {
    if (argc < 3) {
        vga_print("usage: write <file> <text...>\n");
        return 1;
    }

    char *buf = kmalloc(WRITE_BUF);
    if (!buf) {
        vga_print("write: out of memory\n");
        return 1;
    }

    uint32_t pos = 0;
    for (int i = 2; i < argc; i++) {
        int len = kstrlen(argv[i]);
        if (pos + len + 2 >= WRITE_BUF) break;
        kstrncpy(buf + pos, argv[i], len);
        pos += len;
        if (i < argc - 1)
            buf[pos++] = ' ';
    }
    buf[pos++] = '\n';
    buf[pos] = '\0';

    if (ramfs_write(argv[1], buf, pos) < 0) {
        vga_print("write: failed (filesystem full?)\n");
        kfree(buf);
        return 1;
    }

    kfree(buf);
    return 0;
}