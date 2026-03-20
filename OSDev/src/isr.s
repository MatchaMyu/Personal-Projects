BITS 32
GLOBAL idt_load

;Already NASM-style, no need to refer to .global and .extern

; ISR load...
GLOBAL isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
GLOBAL isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
GLOBAL isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
GLOBAL isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

; IRQ load...
GLOBAL irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
GLOBAL irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15


extern isr_common_handler

; --------
; Macros
; --------
%macro ISR_NOERR 1
isr%1:
    cli
    push dword 0        ; fake error code
    push dword %1       ; vector number
    jmp interrupt_common_stub
%endmacro

%macro ISR_ERR 1
isr%1:
    cli
    push dword %1       ; vector number (CPU already pushed error code)
    jmp interrupt_common_stub
%endmacro

%macro IRQ 2
irq%1:
    cli
    push dword 0
    push dword %2
    jmp interrupt_common_stub
%endmacro

; ------------------------
; Common stub
; Stack on entry:
;  - For NOERR: [vec][err=0] already pushed by us
;  - For ERR:   [vec] pushed by us, and CPU pushed [err] earlier
; ------------------------
interrupt_common_stub:
    pusha                   ; EDI,ESI,EBP,ESP,EBX,EDX,ECX,EAX (in that order on stack)
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10            ; your kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                ; arg: pointer to register frame
    call isr_common_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8              ; drop [vector] and [error_code]
    iret

; ------------------------
; Define ISRs
; ------------------------
; No error code:
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

; With error code:
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15

ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; -------------
;Define IRQs
;-----------

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

;----------
; Load IDT
;---------
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret


