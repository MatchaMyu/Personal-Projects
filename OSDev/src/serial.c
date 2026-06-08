#include "serial.h"
#include "ports.h"

#define COM1 0x3F8

void serial_init(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

static int serial_ready(void) {
    return inb(COM1 + 5) & 0x20;
}

static void serial_putc(char c) {
    while (!serial_ready()) {}
    outb(COM1, c);
}

void serial_write(const char* s) {
    while (*s) {
        serial_putc(*s++);
    }
}

void serial_write_hex32(uint32_t value) {
    char hex[] = "0123456789ABCDEF";
    serial_write("0x");

    for (int i = 28; i >= 0; i -= 4) {
        serial_putc(hex[(value >> i) & 0xF]);
    }
}
