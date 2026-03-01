#include "idt.h"

#define IDT_ENTRIES 256
static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t idt_ptr;

void idt_install_exceptions(void); //forward declaration

extern void idt_load(const idt_ptr_t* ptr);   // asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void idt_set_gate(uint8_t vec, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[vec].offset_low  = (uint16_t)(handler & 0xFFFF);
    idt[vec].selector    = sel;
    idt[vec].zero        = 0;
    idt[vec].type_attr   = flags;            // present | ring0 | 32-bit interrupt gate
    idt[vec].offset_high = (uint16_t)((handler >> 16) & 0xFFFF);
}

void idt_init(void) {
    // Zero the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt[i] = (idt_entry_t){0};
    }

    idt_ptr.limit = (uint16_t)(sizeof(idt) - 1);
    idt_ptr.base  = (uint32_t)&idt;

    idt_install_exceptions();
    idt_load(&idt_ptr);
}

void idt_install_exceptions(void)
{
    idt_set_gate(0,  (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10,  (uint32_t)isr10,  0x08, 0x8E);
    idt_set_gate(11,  (uint32_t)isr11,  0x08, 0x8E);
    idt_set_gate(12,  (uint32_t)isr12,  0x08, 0x8E);
    idt_set_gate(13,  (uint32_t)isr13,  0x08, 0x8E);
    idt_set_gate(14,  (uint32_t)isr14,  0x08, 0x8E);
    idt_set_gate(15,  (uint32_t)isr15,  0x08, 0x8E);
    idt_set_gate(16,  (uint32_t)isr16,  0x08, 0x8E);
    idt_set_gate(17,  (uint32_t)isr17,  0x08, 0x8E);
    idt_set_gate(18,  (uint32_t)isr18,  0x08, 0x8E);
    idt_set_gate(19,  (uint32_t)isr19,  0x08, 0x8E);
    idt_set_gate(20,  (uint32_t)isr20,  0x08, 0x8E);
    idt_set_gate(21,  (uint32_t)isr21,  0x08, 0x8E);
    idt_set_gate(22,  (uint32_t)isr22,  0x08, 0x8E);
    idt_set_gate(23,  (uint32_t)isr23,  0x08, 0x8E);
    idt_set_gate(24,  (uint32_t)isr24,  0x08, 0x8E);
    idt_set_gate(25,  (uint32_t)isr25,  0x08, 0x8E);
    idt_set_gate(26,  (uint32_t)isr26,  0x08, 0x8E);
    idt_set_gate(27,  (uint32_t)isr27,  0x08, 0x8E);
    idt_set_gate(28,  (uint32_t)isr28,  0x08, 0x8E);
    idt_set_gate(29,  (uint32_t)isr29,  0x08, 0x8E);
    idt_set_gate(30,  (uint32_t)isr31,  0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
}
