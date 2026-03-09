#include "vga.h"

static int cursor_col = 0;
static int cursor_row = 0;

void vga_clear() {
    char *video = (char *)VGA_ADDRESS;

    for (int i = 0; i < VGA_COLS * VGA_ROWS * 2; i += 2) {
        video[i] = ' ';
        video[i+1] = 0x07;
    }

    cursor_col = 0;
    cursor_row = 0;
}

void vga_putchar(char c) {
    char *video = (char *)VGA_ADDRESS;

    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        return;
    }

    if (cursor_col >= VGA_COLS) return;

    int offset = (cursor_row * VGA_ROWS + cursor_col) * 2;
    video[offset] = c;
    video[offset+1] = 0x07;

    cursor_col++;
}

void vga_print(const char *str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}