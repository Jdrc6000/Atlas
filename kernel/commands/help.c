#include "shell.h"
#include "vga.h"

extern const command_t *registry;
extern int registry_len;

int cmd_help(int argc, char **argv) {
    (void)argc;
    (void)argv;

    vga_print("available commands:\n");

    for (int i = 0; i < registry_len; i++) {
        vga_print("  ");
        vga_print(registry[i].name);
        vga_print(" - ");
        vga_print(registry[i].description);
        vga_putchar('\n');
    }

    return 0;
}