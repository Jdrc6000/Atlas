#include "irq.h"
#include "idt.h"
#include "pic.h"
#include "vga.h"
#include "keyboard.h"

extern void irq0_stub();
extern void irq1_stub();

#define PIT_CHANNEL0 0x40
#define PIT_CMD 0x43
#define PIT_CLOCK 1193180

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void pit_set_frequency(uint32_t hz) {
    uint32_t divisor = PIT_CLOCK / hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_CHANNEL0, (unsigned char)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (unsigned char)((divisor >> 8) & 0xFF));
}

static volatile uint32_t ticks = 0;

uint32_t irq_get_ticks() { return ticks; }

void irq0_handler() {
    ticks++;
    pic_send_eoi(0);
}

void irq1_handler() {
    char c = keyboard_poll();
    if (c != 0)
        keyboard_push(c);
    pic_send_eoi(1);
}

void irq_init() {
    pic_remap(0x20, 0x28);
    pit_set_frequency(TICKS_PER_SEC);
    idt_set_gate(0x20, (uint32_t)irq0_stub, 0x08, 0x8E); // timer
    idt_set_gate(0x21, (uint32_t)irq1_stub, 0x08, 0x8E); //keyboard
    irq_clear_mask(0);
    irq_clear_mask(1);
    __asm__ volatile ("sti");
}