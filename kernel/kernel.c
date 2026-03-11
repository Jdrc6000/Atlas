#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "irq.h"
#include "idt.h"

extern int cmd_clear(int argc, char **argv);
extern int cmd_echo(int argc, char **argv);
extern int cmd_help(int argc, char**argv);
extern int cmd_uptime(int argc, char **argv);

static const command_t commands[] = {
    { "clear", "Clear the screen", cmd_clear },
    { "echo", "Print arguments", cmd_echo },
    { "help", "List all commands", cmd_help },
    { "uptime", "Show time since boot", cmd_uptime }
};

void kernel_main() {
    vga_clear();
    vga_print("Atlas OS\n");

    idt_init();
    irq_init();

    keyboard_init();
    shell_register_commands(commands, sizeof(commands) / sizeof((commands)[0]));
    shell_run();
}