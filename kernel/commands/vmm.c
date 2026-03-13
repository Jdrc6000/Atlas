#include "vga.h"
#include "vmm.h"
#include "pmm.h"
#include "paging.h"

int cmd_vmm(int argc, char **argv) {
    (void)argc; (void)argv;

    uint32_t *pd = paging_get_directory();

    uint32_t phys = pmm_alloc();
    vga_print("phys page:  0x"); vga_print_hex(phys); vga_putchar('\n');

    uint32_t virt = 0x400000;
    vmm_map(pd, virt, phys, PAGE_PRESENT | PAGE_WRITE);

    uint32_t *ptr = (uint32_t *)virt;
    *ptr = 0xDEADBEEF;

    vga_print("wrote 0xDEADBEEF to 0x400000\n");
    vga_print("read back: 0x"); vga_print_hex(*ptr); vga_putchar('\n');

    uint32_t resolved = vmm_get_phys(pd, virt);
    vga_print("resolved:   0x"); vga_print_hex(resolved); vga_putchar('\n');

    vmm_unmap(pd, virt);
    vga_print("unmapped\n");

    return 0;
}