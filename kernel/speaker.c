#include "speaker.h"
#include "irq.h"

#define PIT_CHANNEL2 0x42
#define PIT_CMD 0x43
#define SPEAKER_PORT 0x61
#define PIT_CLOCK 1193180

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void speaker_play(uint32_t hz) {
    if (hz == 0) return;

    uint32_t divisor = PIT_CLOCK / hz;

    outb(PIT_CMD, 0xB6);
    outb(PIT_CHANNEL2, (unsigned char)(divisor & 0xFF));
    outb(PIT_CHANNEL2, (unsigned char)((divisor >> 8) & 0xFF));

    unsigned char val = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, val | 0x03);
}

void speaker_stop() {
    // disconnect speaker by clearing bits 0 and 1
    unsigned char val = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, val & ~0x03);
}

void speaker_beep(uint32_t hz, uint32_t ms) {
    uint32_t ticks = (ms * 182) / 10000;
    if (ticks == 0) ticks = 1;

    uint32_t start = irq_get_ticks();
    speaker_play(hz);
    while (irq_get_ticks() - start < ticks);
    speaker_stop();
}