[bits 32]

extern kernel_main

call_kernel:
    call kernel_main
    jmp $