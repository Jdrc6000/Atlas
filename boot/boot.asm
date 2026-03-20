[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

start:
    mov [BOOT_DRIVE], dl

    mov bp, 0x9000
    mov sp, bp

    call load_kernel
    call switch_to_pm

    jmp $

; register / purpose
; AH / function name
; CH / cylinder
; CL / sector number
; DH / head
; DL / drive number
; BX / memory address buffer
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 128  ; number of sectors to load
    mov dl, [BOOT_DRIVE]

    mov ah, 0x02
    mov al, dh
    mov ch, 0
    mov cl, 2
    mov dh, 0
    int 0x13
    ret

gdt_start:
    dd 0x0
    dd 0x0

    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

; protected mode
switch_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or  eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    jmp KERNEL_OFFSET  ; jump to loaded kernel

[bits 16]
BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xAA55