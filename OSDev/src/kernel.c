//Purpose: Main kernel logic entry point from kernel_main (links to Boot.s)
//Contains minor error printing and handling.

#include <stdint.h>
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "vga.h"
#include "timer.h"
#include "keyboard.h"
#include "shell/shell.h"

static void dump_idtr(int row);
static void draw_boot_screen(void);

static inline void trigger_bp(void) {
    __asm__ __volatile__("int3");
}

void kernel_main(void) {
    vga_clear(0x0F);

    // light magenta / pink-ish text
    vga_print_at("Hello from MyuOS!", 0x0D, 0, 0);

    vga_print_at("Before idt_init()", 0x0D, 0, 1);
    dump_idtr(2); //Prints IDT table on row 2.

    idt_init();

    vga_print_at("After idt_init()", 0x0D, 0, 4);
    dump_idtr(5); //prints IDT on row 5.

    idt_install_exceptions();
    vga_print_at("After idt_install_exceptions()", 0x0F, 0, 7);

    irq_install();
    vga_print_at("After irq_install()", 0x0F, 0, 9);

    vga_print_at("Before STI", 0x0F, 0, 10);
    __asm__ volatile ("sti");
    vga_print_at("After STI", 0x0F, 0, 11);

    vga_print_at("Before INT3", 0x0F, 0, 12);
    __asm__ volatile ("int3");
    vga_print_at("After INT3", 0x0F, 0, 13);

    //vga_print_at("Triggering #DE now...", 0x0F, 5, 6);
    //#UD Fault
    //vga_print_at("Crashing UD2", 0x0D, 0, 12);
    //__asm__ __volatile__("ud2");

    vga_clear(0x0F);
    timer_init(100);
    draw_boot_screen();
    draw_uptime();
    shell_run();

    // halt forever
    for (;;) { __asm__ __volatile__("hlt"); }

}

struct __attribute__((packed)) idtr32 {
    uint16_t limit;
    uint32_t base;
};

static void dump_idtr(int row) {
    struct idtr32 idtr;
    __asm__ __volatile__("sidt %0" : "=m"(idtr));

    vga_print_at("IDTR base:", 0x0E, 0, row);
    vga_print_hex32(idtr.base, 0x0F, 11, row);

    vga_print_at("IDTR limit:", 0x0E, 0, row+1);
    vga_print_hex16(idtr.limit, 0x0F, 12, row+1);
}

void isr3_handler_c(void) {
    vga_print_at("ISR3: int3 reached!", 0x0A, 0, 10);
}

void draw_boot_screen(void) {
    vga_print_at("================================", 0x0F, 0, 1);
    vga_print_at("          EquineOS v0.01           ", 0x0D, 0, 2);
    vga_print_at("================================", 0x0F, 0, 3);

    vga_print_at("Boot complete.", 0x0F, 0, 5);
    vga_print_at("Interrupts: enabled", 0x0F, 0, 6);
    vga_print_at("Ready for keyboard input.", 0x0F, 0, 7);

}
