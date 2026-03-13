#include "keyboard.h"
#include "vga.h"

#define KB_BUF_SIZE 64

static char scancode_map[128] = {
    0,
    0,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`',
    0,
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,
    0,
    0,
    ' ',
};

static char scancode_map_shifted[128] = {
    0,
    0,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b',
    '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n',
     0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    '~',
     0,
    '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0,
    0,
    0,
    ' ',
};

static int shift_held = 0;
static int caps_lock_on = 0;
static volatile char kb_buf[KB_BUF_SIZE];
static volatile int kb_head = 0;
static volatile int kb_tail = 0;

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void keyboard_init() {
    while (inb(0x64) & 0x01)
        inb(0x60);
}

static int e0_prefix = 0;

char keyboard_poll() {
    if (!(inb(0x64) & 0x01)) return 0;
    unsigned char scancode = inb(0x60);

    if (scancode == 0xE0) { e0_prefix = 1; return 0; }

    if (e0_prefix) {
        e0_prefix = 0;

        if (scancode == 0x48) return KEY_UP;
        if (scancode == 0x50) return KEY_DOWN;
        if (scancode == 0x4B) return KEY_LEFT;
        if (scancode == 0x4D) return KEY_RIGHT;
        if (scancode == 0x53) return '\x7F';

        return 0;
    }

    int released = scancode & 0x80;
    unsigned char key = scancode & 0x7F;

    if (key == 0x2A || key == 0x36) { shift_held = !released; return 0; }
    if (key == 0x3A) { if (!released) caps_lock_on = !caps_lock_on; return 0; }
    if (released) return 0;

    char base = scancode_map[key];

    if (base == 0) return 0;
    int is_letter = (base >= 'a' && base <= 'z');
    if (is_letter) {
        int uppercase = caps_lock_on ^ shift_held;
        return uppercase ? (base - 'a' + 'A') : base;
    } else {
        return shift_held ? scancode_map_shifted[key] : base;
    }
}

void keyboard_push(char c) {
    int next = (kb_head + 1) % KB_BUF_SIZE;
    if (next == kb_tail) return;
    kb_buf[kb_head] = c;
    kb_head = next;
}

int keyboard_haschar() {
    return kb_head != kb_tail;
}

char keyboard_getchar() {
    if (!keyboard_haschar())
        return 0;
    char c = kb_buf[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUF_SIZE;
    return c;
}