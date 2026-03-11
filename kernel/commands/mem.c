#include "vga.h"
#include "kmalloc.h"

int cmd_mem(int argc, char **argv) {
    (void)argc;
    (void)argv;
    vga_print("heap used: ");
    vga_print_int(kmalloc_used());
    vga_print(" bytes\n");
    return 0;
}