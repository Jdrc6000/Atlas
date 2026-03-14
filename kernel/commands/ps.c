#include "vga.h"
#include "task.h"

static const char *state_name(task_state_t s) {
    switch (s) {
        case TASK_RUNNING: return "running";
        case TASK_READY: return "ready";
        case TASK_SLEEPING: return "sleeping";
        case TASK_DEAD: return "dead";
        default: return "?";
    }
}

int cmd_ps(int argc, char **argv) {
    (void)argc; (void)argv;
    vga_print("PID  STATE\n");
    vga_print("---  --------\n");
    int count = task_count_get();
    for (int i = 0; i < count; i++) {
        task_t *t = task_get(i);
        vga_print_int(t->id);
        vga_print("    ");
        vga_print(state_name(t->state));
        vga_putchar('\n');
    }
    return 0;
}