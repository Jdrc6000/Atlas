#include "vga.h"
#include "irq.h"

#define TICKS_PER_SEC 18

int cmd_uptime(int argc, char **argv) {
    (void)argc;
    (void)argv;
    unsigned int ticks = irq_get_ticks();
    unsigned int seconds = ticks / TICKS_PER_SEC;
    unsigned int minutes = seconds / 60;
    unsigned int hours   = minutes / 60;
    seconds %= 60;
    minutes %= 60;

    vga_print("uptime: ");
    vga_print_int(hours);
    vga_print("h ");
    vga_print_int(minutes);
    vga_print("m ");
    vga_print_int(seconds);
    vga_print("s\n");
    return 0;
}