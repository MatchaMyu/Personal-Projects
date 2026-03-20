#pragma once
#include <stdint.h>

/* Basic character output */
void vga_putc_at(char c, uint8_t color, int x, int y);

/* String output */
void vga_print_at(const char* s, uint8_t color, int x, int y);

void vga_put_char(char c);

/* Hex helpers */
void vga_print_hex32(uint32_t v, uint8_t color, int x, int y);
void vga_print_hex16(uint16_t v, uint8_t color, int x, int y);

void u32_to_string(uint32_t value, char* str);
void vga_print_uint_at(uint32_t value, uint8_t color, int x, int y);

/* Screen control */
void vga_clear(uint8_t color);
void vga_set_cursor_pos(int x, int y);
