#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "kmalloc.h"

#define HISTORY_MAX 16

const command_t *registry = 0;
int registry_len = 0;

char input_buf[MAX_INPUT];
int buf_len = 0;

static char *history[HISTORY_MAX];
static int history_len = 0;
static int history_head = 0;
static int history_pos = -1; // -1 = not navigating

// compares two strings
static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == *b;
}

static int strlen(const char *s) {
    int n = 0; while (s[n]) n++; return n;
}

void shell_register_commands(const command_t *cmds, int count) {
    registry = cmds;
    registry_len = count;
}

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

static void history_push(const char *cmd) {
    // free oldest slot if full
    if (history[history_head])
        history[history_head] = 0; // the only problem with bump alloc = cant free memory
    history[history_head] = kstrdup(cmd);
    history_head = (history_head + 1) % HISTORY_MAX;
    if (history_len < HISTORY_MAX) history_len++;
}

static char *history_get(int offset) {
    if (offset < 0 || offset >= history_len) return 0;
    int idx = (history_head - 1 - offset + HISTORY_MAX * 2) % HISTORY_MAX;
    return history[idx];
}

static void redraw_line(const char *new_buf, int new_len) {
    // erase current input with backspaces
    for (int i = 0; i < buf_len; i++)
        vga_putchar('\b');
    
    // print new content
    for (int i = 0; i < new_len; i++)
        vga_putchar(new_buf[i]);
    
    // copy into input_buf
    for (int i = 0; i < new_len; i++)
        input_buf[i] = new_buf[i];
    buf_len = new_len;
}

static void shell_prompt() {
    vga_print("atlas>");
}

void shell_run() {
    shell_prompt();
    
    while (1) {
        if (!keyboard_haschar()) continue;
        char c = keyboard_getchar();

        if (c == '\n') {
            input_buf[buf_len] = '\0';
            vga_putchar('\n');
            if (buf_len > 0) {
                history_push(input_buf);
                history_pos = -1;
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

        } else if (c == KEY_UP) {
            int next = history_pos + 1;
            char *entry = history_get(next);
            if (entry) {
                history_pos = next;
                redraw_line(entry, strlen(entry));
            }

        } else if (c == KEY_DOWN) {
            int next = history_pos - 1;
            if (next < 0) {
                // back to empty line
                redraw_line("", 0);
                history_pos = -1;
            } else {
                char *entry = history_get(next);
                if (entry) {
                    history_pos = next;
                    redraw_line(entry, strlen(entry));
                }
            }

        } else {
            if (buf_len < MAX_INPUT - 1) {
                input_buf[buf_len++] = c;
                vga_putchar(c);
            }
        }
    }
}