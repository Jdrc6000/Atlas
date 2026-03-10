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
KEYBOARD_OBJ := $(BUILD)/keyboard.o
SHELL_OBJ := $(BUILD)/shell.o
KERNEL_BIN := $(BUILD)/kernel.bin
OS_IMG := $(BUILD)/os.img
.PHONY: all
all: $(OS_IMG)
$(OS_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@
$(BOOT_BIN): $(BOOT_SRC) | $(BUILD)
	$(ASM) -f bin $< -o $@
$(KERNEL_BIN): $(ENTRY_OBJ) $(KERNEL_OBJ) $(VGA_OBJ) $(KEYBOARD_OBJ) $(SHELL_OBJ) | $(BUILD)
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
$(BUILD):
	mkdir -p $(BUILD)
.PHONY: run
run: $(OS_IMG)
	qemu-system-i386 -drive format=raw,file=$(OS_IMG)
.PHONY: clean
clean:
	rm -rf $(BUILD)