#include "vga.h"

int cmd_echo(int argc, char **argv) {
    int newline = 1;
    int start = 1;

    if (argc > 1) {
        if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0') {
            newline = 0;
            start = 2;
        }
    }

    for (int i = start; i < argc; i++) {
        vga_print(argv[i]);

        if (i < argc - 1)
            vga_putchar(' ');
    }

    if (newline) vga_putchar('\n');

    return 0;
}