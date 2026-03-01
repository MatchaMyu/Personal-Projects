BITS 32
GLOBAL idt_load
GLOBAL isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
GLOBAL isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
GLOBAL isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
GLOBAL isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

extern isr_common_handler

; --------
; Macros
; --------
%macro ISR_NOERR 1
isr%1:
    cli
    push dword 0        ; fake error code
    push dword %1       ; vector number
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
isr%1:
    cli
    push dword %1       ; vector number (CPU already pushed error code)
    jmp isr_common_stub
%endmacro

; ------------------------
; Common stub
; Stack on entry:
;  - For NOERR: [vec][err=0] already pushed by us
;  - For ERR:   [vec] pushed by us, and CPU pushed [err] earlier
;
; We build a consistent "register frame" and pass a pointer to C.
; ------------------------
isr_common_stub:
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
    ;sti (Still setting up exceptions/IRQs...)
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

; void idt_load(const idt_ptr_t* ptr)
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret
