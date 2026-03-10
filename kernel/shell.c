#include "shell.h"
#include "vga.h"
#include "keyboard.h"

static char input_buf[MAX_INPUT];
static int buf_len = 0;

static void shell_prompt() {
    vga_print("atlas>");
}

static void shell_exec(char *cmd) {
    vga_print("Unknown command\n");
}

void shell_run() {
    shell_prompt();

    while (1) {
        char c = keyboard_poll();
        if (c == 0) continue;

        if (c == '\n') {
            input_buf[buf_len] = '\0';
            vga_putchar('\n');
            if (buf_len > 0)
                shell_exec(input_buf);
            buf_len = 0;
            shell_prompt();
        } else if (c == '\b') {
            if (buf_len > 0) {
                buf_len--;
                vga_putchar('\b');
            }
        } else {
            if (buf_len < MAX_INPUT - 1) {
                input_buf[buf_len++] = c;
                vga_putchar(c);
            }
        }
    }
}