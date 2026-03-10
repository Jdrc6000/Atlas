#include "vga.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main() {
    vga_clear();
    vga_print("Atlas OS\n");
    vga_print("Hello!!\n");

    keyboard_init();
    shell_run();
}