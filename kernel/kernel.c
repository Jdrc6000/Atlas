#include "vga.h"

void kernel_main() {
    vga_clear();
    vga_print("Atlas OS\n");
    vga_print("Hello!!\n");

    while (1)
        ;
}