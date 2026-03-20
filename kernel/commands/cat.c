#include "vga.h"
#include "ramfs.h"
#include "kmalloc.h"

#define CAT_BUF 4096

int cmd_cat(int argc, char **argv) {
    if (argc < 2) {
        vga_print("usage: cat <file>\n");
        return 1;
    }

    char *buf = kmalloc(CAT_BUF);
    if (!buf) {
        vga_print("cat: out of memory\n");
        return 1;
    }

    int n = ramfs_read(argv[1], buf, CAT_BUF);
    if (n < 0) {
        vga_print("cat: no such file: ");
        vga_print(argv[1]);
        vga_putchar('\n');
        kfree(buf);
        return 1;
    }

    vga_print(buf);
    if (n == 0 || buf[n - 1] != '\n')
        vga_putchar('\n');
    kfree(buf);
    
    return 0;
}