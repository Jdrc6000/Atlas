#include "vga.h"
#include "keyboard.h"

void kernel_main() {
    vga_clear();
    vga_print("Atlas OS\n");
    vga_print("Hello!!\n");

    keyboard_init();
    
    while (1) {
        char c = keyboard_poll();
        if (c != 0)
            vga_putchar(c);
    }
}