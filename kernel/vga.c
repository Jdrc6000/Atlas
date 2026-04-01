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

static void vga_scroll() {
    char *video = (char *)VGA_ADDRESS;

    // shift every row up by one
    for (int row = 0; row < VGA_ROWS - 1; row++) {
        for (int col = 0; col < VGA_COLS; col++) {
            int dst = (row * VGA_COLS + col) * 2;
            int src = ((row + 1) * VGA_COLS + col) * 2;
            video[dst]   = video[src];
            video[dst+1] = video[src+1];
        }
    }
    
    // blank out the last row
    for (int col = 0; col < VGA_COLS; col++) {
        int offset = ((VGA_ROWS - 1) * VGA_COLS + col) * 2;
        video[offset]   = ' ';
        video[offset+1] = 0x07;
    }
    cursor_row = VGA_ROWS - 1;
}

void vga_putchar(char c) {
    char *video = (char *)VGA_ADDRESS;
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_ROWS)
            vga_scroll();
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
        int offset = (cursor_row * VGA_COLS + cursor_col) * 2;
        video[offset] = ' ';
        video[offset+1] = 0x07;
        vga_set_cursor(cursor_row, cursor_col);
        return;
    }
    if (cursor_col >= VGA_COLS) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_ROWS)
            vga_scroll();
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

void vga_print_int(int n) {
    if (n == 0) { vga_putchar('0'); return; }
    if (n < 0) {
        vga_putchar('-');
        unsigned int u = (unsigned int)(-(n + 1)) + 1;
        char buf[16];
        int i = 0;
        while (u > 0) {
            buf[i++] = '0' + (u % 10);
            u /= 10;
        }
        for (int j = i - 1; j >= 0; j--)
            vga_putchar(buf[j]);
        return;
    }

    char buf[16];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    
    for (int j = i - 1; j >= 0; j--)
        vga_putchar(buf[j]);
}

void vga_print_hex(uint32_t n) {
    char digits[] = "0123456789abcdef";
    for (int i = 28; i >= 0; i -= 4)
        vga_putchar(digits[(n >> i) & 0xF]);
}

void vga_move_cursor_left() {
    if (cursor_col > 0) cursor_col--;
    else if (cursor_row > 0) { cursor_row--; cursor_col = VGA_COLS - 1; }
    vga_set_cursor(cursor_row, cursor_col);
}