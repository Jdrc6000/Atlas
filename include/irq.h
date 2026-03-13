#ifndef IRQ_H
#define IRQ_H

typedef unsigned int uint32_t;

#define TICKS_PER_SEC 1000

uint32_t irq_get_ticks();
void irq_init();
void irq0_handler();
void irq1_handler();

#endif