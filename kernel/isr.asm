[bits 32]

extern isr_handler
%macro ISR_NOERR 1
global isr%1
isr%1:
    push byte 0
    push byte %1
    jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push byte %1
    jmp isr_common
%endmacro

ISR_NOERR 0 ; #DE divide by zero
ISR_NOERR 1 ; #DB debug
ISR_NOERR 2 ;     NMI
ISR_NOERR 3 ; #BP breakpoint
ISR_NOERR 4 ; #OF overflow
ISR_NOERR 5 ; #BR bound range exceeded
ISR_NOERR 6 ; #UD invalid opcode
ISR_NOERR 7 ; #NM device not available
ISR_ERR   8 ; #DF double fault
ISR_NOERR 9 ;     coprocessor segment overrun (legacy)
ISR_ERR   10 ; #TS invalid TSS
ISR_ERR   11 ; #NP segment not present
ISR_ERR   12 ; #SS stack-segment fault
ISR_ERR   13 ; #GP general protection fault
ISR_ERR   14 ; #PF page fault
ISR_NOERR 15 ;     reserved
ISR_NOERR 16 ; #MF x87 floating-point exception
ISR_ERR   17 ; #AC alignment check
ISR_NOERR 18 ; #MC machine check
ISR_NOERR 19 ; #XM SIMD floating-point exception
ISR_NOERR 20 ; #VE virtualisation exception
ISR_ERR   21 ; #CP control protection exception
ISR_NOERR 22 ;     reserved
ISR_NOERR 23 ;     reserved
ISR_NOERR 24 ;     reserved
ISR_NOERR 25 ;     reserved
ISR_NOERR 26 ;     reserved
ISR_NOERR 27 ;     reserved
ISR_NOERR 28 ; #HV hypervisor injection exception
ISR_ERR   29 ; #VC VMM communication exception
ISR_ERR   30 ; #SX security exception
ISR_NOERR 31 ;     reserved

isr_common:
    pusha ; push edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds
    push eax ; save data segment
    mov ax, 0x10 ; load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; pass pointer to registers_t as argument
    call isr_handler
    add esp, 4 ; pop argument
    pop eax ; restore data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa ; restore general-purpose registers
    add esp, 8 ; pop int_no + err_code
    iret