#include <stdint.h>
#include "ports.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

//Colors.
#define VGA_COLOR_BLACK         0x0
#define VGA_COLOR_BLUE          0x1
#define VGA_COLOR_GREEN         0x2
#define VGA_COLOR_CYAN          0x3
#define VGA_COLOR_RED           0x4
#define VGA_COLOR_MAGENTA       0x5
#define VGA_COLOR_BROWN         0x6
#define VGA_COLOR_LIGHT_GREY    0x7
#define VGA_COLOR_DARK_GREY     0x8
#define VGA_COLOR_LIGHT_BLUE    0x9
#define VGA_COLOR_LIGHT_GREEN   0xA
#define VGA_COLOR_LIGHT_CYAN    0xB
#define VGA_COLOR_LIGHT_RED     0xC
#define VGA_COLOR_LIGHT_MAGENTA 0xD
#define VGA_COLOR_LIGHT_BROWN   0xE
#define VGA_COLOR_WHITE         0xF

static inline uint8_t vga_make_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t cursor_color = 0x0F;

//Getters for cursor
int vga_get_cursor_x(void) {
    return cursor_x;
}

int vga_get_cursor_y(void) {
    return cursor_y;
}

//Cursor enable/Update
void vga_enable_cursor(uint8_t start, uint8_t end);
void vga_disable_cursor(void);

static void vga_update_hw_cursor(void) {
    uint16_t pos = (uint16_t)(cursor_y * VGA_WIDTH + cursor_x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

//Scroll logic
static void vga_scroll(void) {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    // Clear last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            (cursor_color << 8) | ' ';
    }
}

//Wrap Handling (Is it at the bottom?)
static void vga_handle_wrap(void) {
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
	vga_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }
}

//Scroll region
static void vga_set_scroll_region(int top, int bottom);

//Helpers
static inline uint16_t vga_entry(unsigned char ch, uint8_t color) {
    return ((uint16_t)color << 8) | ch;
}

//Character Printing
void vga_putc_at(char c, uint8_t color, int x, int y) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return;
    }

    VGA_MEMORY[y * VGA_WIDTH + x] = vga_entry((uint8_t)c, color);
}

//Line Printing
void vga_print_at(const char* s, uint8_t color, int x, int y) {
    if (!s || y < 0 || y >= VGA_HEIGHT) {
        return;
    }

    int px = x;
    while (*s && px < VGA_WIDTH) {
        if (px >= 0) {
            vga_putc_at(*s, color, px, y);
        }
        s++;
        px++;
    }
}

void u32_to_dec_string(uint32_t value, char* str) {
    int i = 0;
    char temp;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0) {
        str[i++] = '0' + (value % 10);
        value /= 10;
    }

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

void u32_to_hex_string(uint32_t v, char* buf) {
    const char* hex = "0123456789ABCDEF";

    buf[0] = '0';
    buf[1] = 'x';

    for (int i = 0; i < 8; i++) {
        int shift = (7 - i) * 4;
        buf[2 + i] = hex[(v >> shift) & 0xF];
    }

    buf[10] = '\0';
}

static void vga_newline(void) {
    cursor_x = 0;
    cursor_y++;
    vga_handle_wrap();
}

//Typing/Printing logic
void vga_put_char(char c) {
    if (c == '\n') {
        vga_newline();
        vga_update_hw_cursor();
        return;
    } else  if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_putc_at(' ', cursor_color, cursor_x, cursor_y);
        }
	vga_update_hw_cursor();
	return;
    } else if (c == '\t') {
        int spaces = 4 - (cursor_x % 4);
        while (spaces--) {
            vga_put_char(' '); }
            return;
    } else if (c == '\r') {
    cursor_x = 0;
    vga_update_hw_cursor();
    return;
    }

    vga_putc_at(c, cursor_color, cursor_x, cursor_y);
    cursor_x++;
    vga_handle_wrap();
    vga_update_hw_cursor();
}

void vga_set_cursor_pos(int x, int y) {
    if (x < 0) x = 0;
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y < 0) y = 0;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;

    cursor_x = x;
    cursor_y = y;
    vga_update_hw_cursor();
}

void vga_print(const char* str) {
    if (!str) return;
    while (*str) {
        vga_put_char(*str++);
    }
}

void vga_print_uint_at(uint32_t value, uint8_t color, int x, int y)
{
    char buffer[16];
    u32_to_dec_string(value, buffer);
    vga_print_at(buffer, color, x, y);
}

void vga_print_uint(uint32_t value)
{
    char buffer[16];
    u32_to_dec_string(value, buffer);
    vga_print(buffer);
}

void vga_print_hex32(uint32_t value, uint8_t color, int x, int y) {
    char buffer[11];
    u32_to_hex_string(value, buffer);
    vga_print_at(buffer, color, x, y);
}

void vga_print_hex32_cursor(uint32_t value) {
    char buffer[11];
    u32_to_hex_string(value, buffer);
    vga_print(buffer);
}

void vga_print_dec(uint32_t value) {
    char buffer[16];
    int i = 0;

    if (value == 0) {
        vga_print("0");
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0) {
        vga_put_char(buffer[--i]);
    }
}

//Clear screen function.
void vga_clear(uint8_t color) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_putc_at(' ', color, x, y);
        }
    }

    cursor_x = 0;
    cursor_y = 0;
    cursor_color = color;
    vga_update_hw_cursor();
}
