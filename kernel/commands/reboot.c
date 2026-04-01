#include "vga.h"

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

int cmd_reboot(int argc, char **argv) {
    (void)argc; (void)argv;
    vga_print("rebooting...\n");
    
    outb(0x64, 0xFE);

    while (1) __asm__ volatile("hlt");
    return 0;
}