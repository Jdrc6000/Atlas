#include "shell.h"
#include "vga.h"
#include "keyboard.h"

// compares to strings
static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == *b;
}

const command_t *registry = 0;
int registry_len = 0;

void shell_register_commands(const command_t *cmds, int count) {
    registry = cmds;
    registry_len = count;
}

char input_buf[MAX_INPUT];
int buf_len = 0;

static int parse(char *buf, char **argv) {
    int argc = 0;
    char *p = buf;

    while (*p) {
        // skip whitespace
        while (*p == ' ') p++;
        if (!*p) break;

        // start of token
        argv[argc++] = p;
        if (argc >= MAX_ARGS) break;

        while (*p && *p != ' ') p++;
        if (*p == ' ') *p++ = '\0'; // null terminate
    }

    return argc;
}

int shell_exec(int argc, char **argv) {
    if (argc == 0) return 0;

    for (int i = 0; i < registry_len; i++) {
        if (strcmp(registry[i].name, argv[0]))
            return registry[i].handler(argc, argv);
    }

    vga_print("unknown command: ");
    vga_print(argv[0]);
    vga_putchar('\n');
    return 1;
}

static void shell_prompt() {
    vga_print("atlas>");
}

void shell_run() {
    shell_prompt();

    while (1) {
        char c = keyboard_poll();
        if (c == 0) continue;

        if (c == '\n') {
            input_buf[buf_len] = '\0';
            vga_putchar('\n');

            if (buf_len > 0) {
                char *argv[MAX_ARGS];
                int argc = parse(input_buf, argv);
                shell_exec(argc, argv);
            }

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