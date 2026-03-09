#include "keyboard.h"
#include "vga.h"

static char scancode_map[128] = {
    0, 0,'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0,'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,0, 0, ' ',
};

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void keyboard_init() {
    while (inb(0x64) & 0x01)
        inb(0x60);
}

char keyboard_poll() {
    if (!(inb(0x64) & 0x01))
        return 0;
    
    unsigned char scancode = inb(0x60);

    // ignore key release events
    if (scancode & 0x80)
        return 0;
    
    return scancode_map[scancode];
}