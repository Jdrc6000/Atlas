#include "vga.h"

static int cursor_col = 0;
static int cursor_row = 0;

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void vga_set_cursor(int row, int col) {
    unsigned short pos = row * VGA_COLS + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void vga_clear() {
    char *video = (char *)VGA_ADDRESS;

    for (int i = 0; i < VGA_COLS * VGA_ROWS * 2; i += 2) {
        video[i] = ' ';
        video[i+1] = 0x07;
    }

    cursor_col = 0;
    cursor_row = 0;
    vga_set_cursor(0, 0);
}

void vga_putchar(char c) {
    char *video = (char *)VGA_ADDRESS;

    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        vga_set_cursor(cursor_row, cursor_col);
        return;
    }

    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
        } else if (cursor_row > 0) {
            cursor_row--;
            cursor_col = VGA_COLS - 1;
        }

        char *video = (char *)VGA_ADDRESS;
        int offset = (cursor_row * VGA_COLS + cursor_col) * 2;
        video[offset] = ' ';
        video[offset+1] = 0x07;
        vga_set_cursor(cursor_row, cursor_col);
        return;
    }

    if (cursor_col >= VGA_COLS) {
        cursor_col = 0;
        cursor_row++;
    }

    int offset = (cursor_row * VGA_COLS + cursor_col) * 2;
    video[offset] = c;
    video[offset+1] = 0x07;

    cursor_col++;
    vga_set_cursor(cursor_row, cursor_col);
}

void vga_print(const char *str) {
    while (*str) {
        vga_putchar(*str);
        str++;
    }
}