#include "keyboard.h"
#include "vga.h"

static char scancode_map[128] = {
    0, 0,'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    0,'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,0, 0, ' ',
};

static char scancode_map_shifted[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(' ,')' ,'_', '+', '\b',
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', 0, '|',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' ',
};

static int shift_held = 0;
static int caps_lock_on = 0;

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
    int released = scancode & 0x80;
    unsigned char key = scancode & 0x7F; // strip release bit

    if (key == 0x2A || key == 0x36) {
        shift_held = !released;
        return 0;
    }

    if (key == 0x3A) {
        if (!released)
            caps_lock_on = !caps_lock_on;
        return 0;
    }

    if (released)
        return 0;
    
    char base = scancode_map[key];
    if (base == 0)
        return 0;

    int is_letter = (base >= 'a' && base <= 'z');

    if (is_letter) {
        int uppercase = caps_lock_on ^ shift_held;
        return uppercase ? (base - 'a' + 'A') : base;
    } else {
        return shift_held ? scancode_map_shifted[key] : base;
    }
}