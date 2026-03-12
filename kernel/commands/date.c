#include "vga.h"
#include "rtc.h"

static void print2(int n) {
    vga_putchar('0' + n / 10);
    vga_putchar('0' + n % 10);
}

int cmd_date(int argc, char **argv) {
    (void)argc;
    (void)argv;

    rtc_time_t t;
    rtc_read(&t);

    // 20YY-MM-DD HH:MM:SS
    vga_print("20"); // real dirty
    print2(t.year);
    vga_putchar('-');
    print2(t.month);
    vga_putchar('-');
    print2(t.day);
    vga_putchar(' ');
    print2(t.hour);
    vga_putchar(':');
    print2(t.minute);
    vga_putchar(':');
    print2(t.second);
    vga_putchar('\n');

    return 0;
}