[bits 32]

extern irq0_handler
extern irq1_handler

global irq0_stub
global irq1_stub

%macro IRQ_STUB 2
irq%1_stub:
    pusha  ; save all general purpose registers
    call irq%2_handler  ; call the C handler
    popa  ; restore registers
    iret  ; return from interrupt
%endmacro

IRQ_STUB 0, 0
IRQ_STUB 1, 1