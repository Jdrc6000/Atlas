#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT 256
#define MAX_ARGS 16

typedef struct {
    const char *name;
    const char *description;
    int (*handler)(int argc, char **argv);
} command_t;

void shell_run();
void shell_register_commands(const command_t *cmds, int count);
int shell_exec(int argc, char **argv);

#endif