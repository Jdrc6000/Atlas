#include "task.h"
#include "kmalloc.h"
#include "kstring.h"
#include "irq.h"

static task_t tasks[MAX_TASKS];
static int task_count = 0;
static int current_idx = 0;

int task_count_get() { return task_count; }
task_t *task_get(int i) { return &tasks[i]; }

task_t *task_current() {
    return &tasks[current_idx];
}

void task_init() {
    tasks[0].id = 0;
    tasks[0].state = TASK_RUNNING;
    tasks[0].stack = 0;
    tasks[0].esp = 0;
    tasks[0].sleep_until = 0;
    task_count = 1;
    current_idx = 0;
}

task_t *task_create(void (*func)()) {
    if (task_count >= MAX_TASKS) return 0;

    task_t *t = &tasks[task_count];
    t->id = task_count;
    t->state = TASK_READY;
    t->sleep_until = 0;
    t->stack = (uint32_t *)kmalloc(STACK_SIZE);
    if (!t->stack) return 0;

    kmemset(t->stack, 0, STACK_SIZE);
    uint32_t *sp = (uint32_t *)((char *)t->stack + STACK_SIZE);

    *--sp = 0x00000202; // EFLAGS
    *--sp = 0x08; // CS
    *--sp = (uint32_t)func;
    *--sp = 0; // EAX
    *--sp = 0; // ECX
    *--sp = 0; // EDX
    *--sp = 0; // EBX
    *--sp = 0; // ESP
    *--sp = 0; // EBP
    *--sp = 0; // ESI
    *--sp = 0; // EDI

    t->esp = (uint32_t)sp;
    task_count++;
    return t;
}

uint32_t task_schedule(uint32_t current_esp) {
    uint32_t now = irq_get_ticks();

    tasks[current_idx].esp = current_esp;
    if (tasks[current_idx].state == TASK_RUNNING)
        tasks[current_idx].state = TASK_READY;

    // wake any sleeping tasks whose timer has expired
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].state == TASK_SLEEPING && now >= tasks[i].sleep_until)
            tasks[i].state = TASK_READY;
    }

    // round-robin over ready tasks
    int next = current_idx;
    for (int i = 1; i <= task_count; i++) {
        int candidate = (current_idx + i) % task_count;
        if (tasks[candidate].state == TASK_READY) {
            next = candidate;
            break;
        }
    }

    tasks[next].state = TASK_RUNNING;
    current_idx = next;
    return tasks[next].esp;
}

void task_sleep(uint32_t ms) {
    uint32_t ticks = (ms * TICKS_PER_SEC) / 1000;
    if (ticks == 0) ticks = 1;
    tasks[current_idx].sleep_until = irq_get_ticks() + ticks;
    tasks[current_idx].state = TASK_SLEEPING;
    // yield: spin until the scheduler wakes us
    while (tasks[current_idx].state == TASK_SLEEPING)
        __asm__ volatile("hlt");
}

int task_kill(int id) {
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            if (i == 0) return -1; // can't kill kernel task
            tasks[i].state = TASK_DEAD;
            return 0;
        }
    }
    return -1; // not found
}