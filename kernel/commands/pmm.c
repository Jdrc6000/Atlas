#include "vga.h"
#include "pmm.h"

int cmd_pmm(int argc, char **argv) {
    (void)argc; (void)argv;
    uint32_t a = pmm_alloc();
    uint32_t b = pmm_alloc();
    vga_print("alloc 1: 0x"); vga_print_hex(a); vga_putchar('\n');
    vga_print("alloc 2: 0x"); vga_print_hex(b); vga_putchar('\n');
    pmm_free(a);
    uint32_t c = pmm_alloc();
    vga_print("after free+alloc: 0x"); vga_print_hex(c); vga_putchar('\n');
    return 0;
}