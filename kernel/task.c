#include "task.h"
#include "kmalloc.h"
#include "kstring.h"

static task_t tasks[MAX_TASKS];
static int task_count = 0;
static int current_idx = 0;

task_t *task_current() {
    return &tasks[current_idx];
}

void task_init() {
    tasks[0].id = 0;
    tasks[0].state = TASK_RUNNING;
    tasks[0].stack = 0;
    tasks[0].esp = 0;
    task_count = 1;
    current_idx = 0;
}

task_t *task_create(void (*func)()) {
    if (task_count >= MAX_TASKS)
        return 0;

    task_t *t = &tasks[task_count];
    t->id = task_count;
    t->state = TASK_READY;
    t->stack = (uint32_t *)kmalloc(STACK_SIZE);
    if (!t->stack)
        return 0;

    kmemset(t->stack, 0, STACK_SIZE);

    uint32_t *sp = (uint32_t *)((char *)t->stack + STACK_SIZE);

    *--sp = 0x00000202;
    *--sp = 0x08;
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
    tasks[current_idx].esp = current_esp;
    tasks[current_idx].state = TASK_READY;

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