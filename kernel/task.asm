[bits 32]

extern task_schedule
extern irq0_handler
extern irq1_handler

global irq0_stub
global irq1_stub

irq0_stub:
    pusha
    call irq0_handler
    push esp
    call task_schedule
    add  esp, 4
    mov  esp, eax
    popa
    iret

irq1_stub:
    pusha
    call irq1_handler
    popa
    iret