#include "vga.h"
#include "ramfs.h"

static void print_entry(const char *name, uint32_t size) {
    vga_print(name);
    vga_print("\t");
    vga_print_int(size);
    vga_print(" bytes\n");
}

int cmd_ls(int argc, char **argv) {
    (void)argc; (void)argv;
    ramfs_list(print_entry);
    return 0;
}