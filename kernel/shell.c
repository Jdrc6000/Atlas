#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "kmalloc.h"

#define HISTORY_MAX 16

const command_t *registry = 0;
int registry_len = 0;

char input_buf[MAX_INPUT];
int buf_len = 0;

static int cursor_pos = 0;

static char *history[HISTORY_MAX];
static int history_len = 0;
static int history_head = 0;
static int history_pos = -1;

static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == *b;
}
static int strlen(const char *s) {
    int n = 0; while (s[n]) n++; return n;
}

static void redraw_tail() {
    for (int i = cursor_pos; i < buf_len; i++)
        vga_putchar(input_buf[i]);
    vga_putchar(' ');
    int back = buf_len - cursor_pos + 1;
    for (int i = 0; i < back; i++)
        vga_move_cursor_left();
}

void shell_register_commands(const command_t *cmds, int count) {
    registry = cmds;
    registry_len = count;
}

static int parse(char *buf, char **argv) {
    int argc = 0;
    char *p = buf;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[argc++] = p;
        if (argc >= MAX_ARGS) break;
        while (*p && *p != ' ') p++;
        if (*p == ' ') *p++ = '\0';
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
    if (history[history_head])
        kfree(history[history_head]);
    
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
    for (int i = cursor_pos; i < buf_len; i++)
        vga_putchar(input_buf[i]);
    for (int i = 0; i < buf_len; i++)
        vga_putchar('\b');

    for (int i = 0; i < new_len; i++)
        vga_putchar(new_buf[i]);
    
    for (int i = 0; i < new_len; i++)
        input_buf[i] = new_buf[i];
    buf_len = new_len;
    cursor_pos = new_len;
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
            cursor_pos = 0;
            shell_prompt();

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
                redraw_line("", 0);
                history_pos = -1;
            } else {
                char *entry = history_get(next);
                if (entry) {
                    history_pos = next;
                    redraw_line(entry, strlen(entry));
                }
            }

        } else if (c == KEY_LEFT) {
            if (cursor_pos > 0) {
                cursor_pos--;
                vga_move_cursor_left();
            }

        } else if (c == KEY_RIGHT) {
            if (cursor_pos < buf_len) {
                vga_putchar(input_buf[cursor_pos]);
                cursor_pos++;
            }

        } else if (c == '\b') {
            if (cursor_pos > 0) {
                // shift buffer left over the deleted char
                for (int i = cursor_pos - 1; i < buf_len - 1; i++)
                    input_buf[i] = input_buf[i + 1];
                buf_len--;
                cursor_pos--;
                vga_move_cursor_left();
                redraw_tail();
            }

        } else {
            if (buf_len < MAX_INPUT - 1) {
                // shift buffer right to make room
                for (int i = buf_len; i > cursor_pos; i--)
                    input_buf[i] = input_buf[i - 1];
                input_buf[cursor_pos] = c;
                buf_len++;
                cursor_pos++;
                if (cursor_pos < buf_len) {
                    vga_putchar(c);
                    redraw_tail();
                } else {
                    vga_putchar(c);
                }
            }
        }
    }
}