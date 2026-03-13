#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "irq.h"
#include "idt.h"
#include "kmalloc.h"
#include "rtc.h"
#include "paging.h"
#include "pmm.h"

extern int cmd_clear(int argc, char **argv);
extern int cmd_echo(int argc, char **argv);
extern int cmd_help(int argc, char**argv);
extern int cmd_uptime(int argc, char **argv);
extern int cmd_mem(int argc, char **argv);
extern int cmd_date(int argc, char **argv);
extern int cmd_beep(int argc, char **argv);
extern int cmd_pmm(int argc, char **argv);

static const command_t commands[] = {
    { "clear", "Clear the screen", cmd_clear },
    { "echo", "Print arguments", cmd_echo },
    { "help", "List all commands", cmd_help },
    { "uptime", "Show time since boot", cmd_uptime },
    { "mem", "Show bytes in heap", cmd_mem },
    { "date", "Show current date and time", cmd_date },
    { "beep", "Beep the speaker (freq hz, duration ms)", cmd_beep },
    { "pmm", "Test physical memory manager", cmd_pmm },
};

void kernel_main() {
    paging_init();
    pmm_init();

    vga_clear();
    vga_print("Atlas OS\n");

    idt_init();
    irq_init();
    kmalloc_init();
    keyboard_init();

    shell_register_commands(commands, sizeof(commands) / sizeof((commands)[0]));
    shell_run();
}