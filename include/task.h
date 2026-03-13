#ifndef TASK_H
#define TASK_H

typedef unsigned int uint32_t;

#define MAX_TASKS  2
#define STACK_SIZE 4096

typedef enum {
    TASK_RUNNING,
    TASK_READY,
    TASK_DEAD
} task_state_t;

typedef struct {
    uint32_t      esp;
    uint32_t     *stack;
    task_state_t  state;
    int           id;
} task_t;

void    task_init();
task_t *task_create(void (*func)());
task_t *task_current();
uint32_t task_schedule(uint32_t current_esp);

#endif