; Multiboot v1 header + entry that calls C kernel_main
BITS 32

SECTION .multiboot
align 4
dd 0x1BADB002            ; magic
dd 0x00000000            ; flags (keep minimal)
dd -(0x1BADB002 + 0x00000000) ; checksum

SECTION .text
global _start
extern kernel_main

_start:
    ; set up a simple stack
    mov esp, stack_top

    ; call C code
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

SECTION .bss
align 16
stack_bottom:
    resb 16384            ; 16 KiB stack
stack_top:

