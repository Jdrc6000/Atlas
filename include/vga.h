#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000
#define VGA_COLS 80
#define VGA_ROWS 25

void vga_clear();
void vga_putchar(char c);
void vga_print(const char *str);
void set_cursor(int row, int column);

#endif