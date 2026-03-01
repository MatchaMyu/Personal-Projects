//Purpose: Main kernel logic entry point from kernel_main (links to Boot.s)
//Contains minor error printing and handling.

#include <stdint.h>
#include "idt.h"

static void dump_idtr(int row);

static inline void vga_putc_at(char c, uint8_t color, int x, int y) {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    vga[y * 80 + x] = (uint16_t)color << 8 | (uint8_t)c;
}

void vga_print_at(const char* s, uint8_t color, int x, int y) {
    for (int i = 0; s[i] != '\0'; i++) {
        vga_putc_at(s[i], color, x + i, y);
    }
}

static inline void trigger_bp(void) {
    __asm__ __volatile__("int3");
}

void vga_print(const char* s)
{
    // pick reasonable defaults for now
    vga_print_at(s, 0x0F, -1, -1);  // white text, current cursor
}

void kernel_main(void) {
    // clear screen
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            vga_putc_at(' ', 0x0F, x, y);
        }
    }

    // light magenta / pink-ish text
    vga_print_at("Hello from MyuOS!", 0x0D, 0, 0);

    vga_print_at("Before idt_init()", 0x0D, 0, 1);
    dump_idtr(2);

    idt_init();

    vga_print_at("After idt_init()", 0x0D, 0, 4);
    dump_idtr(5);

    vga_print_at("Crashing UD2", 0x0D, 0, 12);
    __asm__ __volatile__("ud2");

   vga_print_at("If you see this, the system did not crash.", 0x0D,0,15);

    // halt forever
    for (;;) { __asm__ __volatile__("hlt"); }

}

struct __attribute__((packed)) idtr32 {
    uint16_t limit;
    uint32_t base;
};

static void vga_print_hex32(uint32_t v, uint8_t color, int x, int y) {
    const char *hex = "0123456789ABCDEF";
    char buf[11];
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 8; i++) buf[2+i] = hex[(v >> (28 - 4*i)) & 0xF];
    buf[10] = 0;
    vga_print_at(buf, color, x, y);
}

static void vga_print_hex16(uint16_t v, uint8_t color, int x, int y) {
    const char *hex = "0123456789ABCDEF";
    char buf[7];
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 4; i++) buf[2+i] = hex[(v >> (12 - 4*i)) & 0xF];
    buf[6] = 0;
    vga_print_at(buf, color, x, y);
}

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
