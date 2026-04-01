#include "isr.h"
#include "vga.h"

static const char *exception_names[] = {
    "Divide By Zero", // 0  #DE
    "Debug", // 1  #DB
    "Non-Maskable Interrupt", // 2
    "Breakpoint", // 3  #BP
    "Overflow", // 4  #OF
    "Bound Range Exceeded", // 5  #BR
    "Invalid Opcode", // 6  #UD
    "Device Not Available", // 7  #NM
    "Double Fault", // 8  #DF
    "Coprocessor Segment Overrun", // 9
    "Invalid TSS", // 10 #TS
    "Segment Not Present", // 11 #NP
    "Stack-Segment Fault", // 12 #SS
    "General Protection Fault", // 13 #GP
    "Page Fault", // 14 #PF
    "Reserved", // 15
    "x87 Floating-Point", // 16 #MF
    "Alignment Check", // 17 #AC
    "Machine Check", // 18 #MC
    "SIMD Floating-Point", // 19 #XM
    "Virtualisation", // 20 #VE
    "Control Protection", // 21 #CP
    "Reserved", // 22
    "Reserved", // 23
    "Reserved", // 24
    "Reserved", // 25
    "Reserved", // 26
    "Reserved", // 27
    "Hypervisor Injection", // 28 #HV
    "VMM Communication", // 29 #VC
    "Security Exception", // 30 #SX
    "Reserved", // 31
};

static uint32_t read_cr2() {
    uint32_t val;
    __asm__ volatile("mov %%cr2, %0" : "=r"(val));
    return val;
}

void isr_handler(registers_t *r) {
    vga_print("\n--- KERNEL PANIC ---\n");

    if (r->int_no < 32)
        vga_print(exception_names[r->int_no]);
    else
        vga_print("Unknown Exception");

    vga_print(" (int ");
    vga_print_int(r->int_no);
    vga_print(")\n");

    vga_print("err_code: 0x");
    vga_print_hex(r->err_code);
    vga_putchar('\n');
    vga_print("eip:      0x");
    vga_print_hex(r->eip);
    vga_putchar('\n');
    vga_print("cs:       0x");
    vga_print_hex(r->cs);
    vga_putchar('\n');
    vga_print("eflags:   0x");
    vga_print_hex(r->eflags);
    vga_putchar('\n');
    vga_print("eax: 0x");
    vga_print_hex(r->eax);
    vga_print("  ebx: 0x");
    vga_print_hex(r->ebx);
    vga_putchar('\n');
    vga_print("ecx: 0x");
    vga_print_hex(r->ecx);
    vga_print("  edx: 0x");
    vga_print_hex(r->edx);
    vga_putchar('\n');
    vga_print("esp: 0x");
    vga_print_hex(r->esp);
    vga_print("  ebp: 0x");
    vga_print_hex(r->ebp);
    vga_putchar('\n');

    // Page fault extra info
    if (r->int_no == 14) {
        uint32_t cr2 = read_cr2();
        vga_print("fault addr: 0x");
        vga_print_hex(cr2);
        vga_putchar('\n');
        vga_print("reason: ");
        vga_print((r->err_code & 0x1) ? "protection " : "not-present ");
        vga_print((r->err_code & 0x2) ? "write " : "read ");
        vga_print((r->err_code & 0x4) ? "user-mode" : "kernel-mode");
        vga_putchar('\n');
    }

    vga_print("system halted.");
    __asm__ volatile("cli; hlt");
}