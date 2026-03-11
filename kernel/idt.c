#include "idt.h"

static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;

void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
}

void idt_init() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    // zero out all 256 entries to start
    for (int i = 0; i < 256; i++)
        idt_set_gate(i, 0, 0, 0);

    // tell the cpu where the idt is
    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}

