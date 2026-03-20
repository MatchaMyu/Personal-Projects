// isr.c
#include "isr.h"
#include "vga.h"
#include "irq.h"

//contains isr_common_handler
//contains crash code.

static const char* exception_names[32] = {
    "0 Divide Error (#DE)",
    "1 Debug (#DB)",
    "2 Non-Maskable Interrupt (NMI)",
    "3 Breakpoint (#BP)",
    "4 Overflow (#OF)",
    "5 Bound Range Exceeded (#BR)",
    "6 Invalid Opcode (#UD)",
    "7 Device Not Available (#NM)",
    "8 Double Fault (#DF)",
    "9 Coprocessor Segment Overrun (legacy)",
    "10 Invalid TSS (#TS)",
    "11 Segment Not Present (#NP)",
    "12 Stack-Segment Fault (#SS)",
    "13 General Protection Fault (#GP)",
    "14 Page Fault (#PF)",
    "15 Reserved",
    "16 x87 Floating-Point Exception (#MF)",
    "17 Alignment Check (#AC)",
    "18 Machine Check (#MC)",
    "19 SIMD Floating-Point Exception (#XM/#XF)",
    "20 Virtualization Exception (#VE)",
    "21 Control Protection Exception (#CP)",
    "22 Reserved",
    "23 Reserved",
    "24 Reserved",
    "25 Reserved",
    "26 Reserved",
    "27 Reserved",
    "28 Hypervisor Injection Exception (#HV) / Reserved on many setups",
    "29 VMM Communication Exception (#VC) / Reserved on many setups",
    "30 Security Exception (#SX)",
    "31 Reserved"
};

isr_t interrupt_handlers[256] = {0};

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void isr_common_handler(regs_t* r)
{
   if (r->int_no == 3) {
    vga_print_at("INT3 BREAKPOINT", 0x2F, 0, 24);
    return;
    }

    if (r->int_no >= 32 && r->int_no < 48) {
        irq_handler(r);
        return;
    }

   //crash/panic
    vga_clear(0x4F);
    vga_print_at("==== SYSTEM HALTED ====", 0x4F, 28, 0);

    uint32_t n = r->int_no;
    if (n < 32) {
        vga_print_at(exception_names[n], 0x0C, 11, 2);
    } else {
        vga_print_at("Unknown exception vector", 0x0C, 11, 2);
    }

    vga_print_at("int_no:", 0x0F, 0, 5);
    vga_print_hex32(r->int_no, 0x0F, 8, 5);

    vga_print_at("err:", 0x0F, 0, 6);
    vga_print_hex32(r->err_code, 0x0F, 8, 6);

    vga_print_at("EIP:", 0x0F, 0, 7);
    vga_print_hex32(r->eip, 0x0F, 8, 7);

    vga_print_at("CS:", 0x0F, 0, 8);
    vga_print_hex32(r->cs, 0x0F, 8, 8);

    vga_print_at("EFLAGS:", 0x0F, 0, 9);
    vga_print_hex32(r->eflags, 0x0F, 8, 9);

    vga_print_at("EAX:", 0x0F, 0, 10); vga_print_hex32(r->eax, 0x0F, 8, 10);
    vga_print_at("EBX:", 0x0F, 0, 11); vga_print_hex32(r->ebx, 0x0F, 8, 11);
    vga_print_at("ECX:", 0x0F, 0, 12); vga_print_hex32(r->ecx, 0x0F, 8, 12);
    vga_print_at("EDX:", 0x0F, 0, 13); vga_print_hex32(r->edx, 0x0F, 8, 13);

    vga_print_at("ESI:", 0x0F, 0, 14); vga_print_hex32(r->esi, 0x0F, 8, 14);
    vga_print_at("EDI:", 0x0F, 0, 15); vga_print_hex32(r->edi, 0x0F, 8, 15);
    vga_print_at("EBP:", 0x0F, 0, 16); vga_print_hex32(r->ebp, 0x0F, 8, 16);
    vga_print_at("ESP:", 0x0F, 0, 17); vga_print_hex32(r->esp, 0x0F, 8, 17);

    vga_print_at("=== SYSTEM HALTED ===", 0x4F, 28, 20);
    for (;;) { __asm__ __volatile__("cli; hlt"); }
}
