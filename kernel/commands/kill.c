#include "vga.h"
#include "task.h"

static int myatoi(const char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9')
        n = n * 10 + (*s++ - '0');
    return n;
}

int cmd_kill(int argc, char **argv) {
    if (argc < 2) {
        vga_print("usage: kill <pid>\n");
        return 1;
    }
    int id = myatoi(argv[1]);
    if (task_kill(id) == 0) {
        vga_print("killed task ");
        vga_print_int(id);
        vga_putchar('\n');
    } else {
        vga_print("kill: no such task (or can't kill task 0)\n");
    }
    return 0;
}