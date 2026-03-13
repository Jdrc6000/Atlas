ASM := nasm
CC := i686-elf-gcc
LD := i686-elf-ld
ASMFLAGS := -f elf32
CFLAGS := -ffreestanding -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra -Iinclude
LDFLAGS := -T linker.ld -m elf_i386 --oformat binary
BUILD := build

BOOT_SRC := boot/boot.asm
ENTRY_SRC := kernel/kernel_entry.asm
KERNEL_C := kernel/kernel.c

BOOT_BIN := $(BUILD)/boot.bin
ENTRY_OBJ := $(BUILD)/kernel_entry.o
KERNEL_OBJ := $(BUILD)/kernel.o
VGA_OBJ := $(BUILD)/vga.o
KEYBOARD_OBJ:= $(BUILD)/keyboard.o
SHELL_OBJ := $(BUILD)/shell.o
CLEAR_OBJ := $(BUILD)/clear.o
ECHO_OBJ := $(BUILD)/echo.o
HELP_OBJ := $(BUILD)/help.o
IDT_OBJ := $(BUILD)/idt.o
PIC_OBJ := $(BUILD)/pic.o
IRQ_OBJ := $(BUILD)/irq.o
ISR_OBJ := $(BUILD)/isr.o
UPTIME_OBJ := $(BUILD)/uptime.o
KMALLOC_OBJ := $(BUILD)/kmalloc.o
MEM_OBJ := $(BUILD)/mem.o
DATE_OBJ := $(BUILD)/date.o
RTC_OBJ := $(BUILD)/rtc.o
SPEAKER_OBJ := $(BUILD)/speaker.o
BEEP_OBJ := $(BUILD)/beep.o
PAGING_OBJ  := $(BUILD)/paging.o
PAGINGASM_OBJ := $(BUILD)/paging_asm.o
PMM_OBJ := $(BUILD)/pmm.o
PMM_CMD_OBJ := $(BUILD)/pmm_cmd.o
VMM_OBJ := $(BUILD)/vmm.o
VMM_CMD_OBJ := $(BUILD)/vmm_cmd.o

KERNEL_BIN := $(BUILD)/kernel.bin
OS_IMG := $(BUILD)/os.img

.PHONY: all
all: $(OS_IMG)

$(OS_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD)
	$(ASM) -f bin $< -o $@

$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ) $(VGA_OBJ) $(KEYBOARD_OBJ) $(SHELL_OBJ) $(CLEAR_OBJ) $(ECHO_OBJ) $(HELP_OBJ) $(IDT_OBJ) $(PIC_OBJ) $(IRQ_OBJ) $(ISR_OBJ) $(UPTIME_OBJ) $(KMALLOC_OBJ) $(MEM_OBJ) $(DATE_OBJ) $(RTC_OBJ) $(SPEAKER_OBJ) $(BEEP_OBJ) $(PAGING_OBJ) $(PAGINGASM_OBJ) $(PMM_OBJ) $(PMM_CMD_OBJ) $(VMM_OBJ) $(VMM_CMD_OBJ) | $(BUILD)
	$(LD) $(LDFLAGS) -o $@ $^

$(VGA_OBJ): kernel/vga.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KEYBOARD_OBJ): kernel/keyboard.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ENTRY_OBJ): $(ENTRY_SRC) | $(BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL_OBJ): $(KERNEL_C) | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELL_OBJ): kernel/shell.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLEAR_OBJ): kernel/commands/clear.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ECHO_OBJ): kernel/commands/echo.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(HELP_OBJ): kernel/commands/help.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(IDT_OBJ): kernel/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(PIC_OBJ): kernel/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(IRQ_OBJ): kernel/irq.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISR_OBJ): kernel/isr.asm | $(BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@

$(UPTIME_OBJ): kernel/commands/uptime.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KMALLOC_OBJ): kernel/kmalloc.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(MEM_OBJ): kernel/commands/mem.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(DATE_OBJ): kernel/commands/date.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(RTC_OBJ): kernel/rtc.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(SPEAKER_OBJ): kernel/speaker.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BEEP_OBJ): kernel/commands/beep.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(PAGING_OBJ): kernel/paging.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(PAGINGASM_OBJ): kernel/paging.asm | $(BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@

$(PMM_OBJ): kernel/pmm.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(PMM_CMD_OBJ): kernel/commands/pmm.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(VMM_OBJ): kernel/vmm.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(VMM_CMD_OBJ): kernel/commands/vmm.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

.PHONY: run
run: $(OS_IMG)
	qemu-system-i386 -drive format=raw,file=$(OS_IMG) -audiodev coreaudio,id=snd -machine pcspk-audiodev=snd

.PHONY: clean
clean:
	rm -rf $(BUILD)