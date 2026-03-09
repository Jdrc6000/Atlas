[bits 32]

global call_kernel
extern kernel_main

call_kernel:
    call kernel_main
    jmp $