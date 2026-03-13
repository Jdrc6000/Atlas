#ifndef VGA_H
#define VGA_H

#define VGA_ADDRESS 0xB8000
#define VGA_COLS 80
#define VGA_ROWS 25

void vga_clear();
void vga_putchar(char c);
void vga_print(const char *str);
void vga_print_int(int n);
void set_cursor(int row, int column);
void vga_move_cursor_left();

#endif