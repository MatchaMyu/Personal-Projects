#pragma once
#include <stdint.h>

/* Basic character output */
void vga_putc_at(char c, uint8_t color, int x, int y);
void vga_put_char(char c);

/* String output */
void vga_print_at(const char* s, uint8_t color, int x, int y);
void vga_print(const char* str);

/* Number / hex output */
void vga_print_hex32(uint32_t v, uint8_t color, int x, int y);
void vga_print_hex32_cursor(uint32_t v);
void vga_print_uint_at(uint32_t value, uint8_t color, int x, int y);
void vga_print_uint(uint32_t value);

/* Cursor position */
int vga_get_cursor_x(void);
int vga_get_cursor_y(void);
void vga_set_cursor_pos(int x, int y);

/* Screen control */
void vga_clear(uint8_t color);
