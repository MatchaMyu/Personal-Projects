#include <stdint.h>

static inline void vga_putc_at(char c, uint8_t color, int x, int y) {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    vga[y * 80 + x] = (uint16_t)color << 8 | (uint8_t)c;
}

static void vga_print(const char* s, uint8_t color, int x, int y) {
    for (int i = 0; s[i] != '\0'; i++) {
        vga_putc_at(s[i], color, x + i, y);
    }
}

void kernel_main(void) {
    // Clear a few lines (optional, but nice)
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            vga_putc_at(' ', 0x0F, x, y); // white on black
        }
    }

    // Print your first kernel message (pink-ish text: light magenta = 0x0D)
    vga_print("Hello from MyuOS! (now halting)", 0x0D, 0, 0);

    // Halt forever
    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}

