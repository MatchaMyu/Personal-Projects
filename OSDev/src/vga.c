#include <stdint.h>
#include "ports.h"

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t cursor_color = 0x0F;

static void vga_update_hw_cursor(void) {
    uint16_t pos = (uint16_t)(cursor_y * 80 + cursor_x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}


//Character Printing
void vga_putc_at(char c, uint8_t color, int x, int y) {
    volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
    vga[y * 80 + x] = (uint16_t)color << 8 | (uint8_t)c;
}

//Line Printing
void vga_print_at(const char* s, uint8_t color, int x, int y) {
    for (int i = 0; s[i] != '\0'; i++) {
        vga_putc_at(s[i], color, x + i, y);
    }
}

void vga_print(const char* s)
{
    // pick reasonable defaults for now
    vga_print_at(s, 0x0F, -1, -1);  // white text, current cursor
}

void vga_print_hex16(uint16_t v, uint8_t color, int x, int y) {
    const char *hex = "0123456789ABCDEF";
    char buf[7];
    buf[0] = '0'; buf[1] = 'x';
    for (int i = 0; i < 4; i++) buf[2+i] = hex[(v >> (12 - 4*i)) & 0xF];
    buf[6] = 0;
    vga_print_at(buf, color, x, y);
}


void vga_print_hex32(uint32_t v, uint8_t color, int x, int y) {
    char buf[11] = "0x00000000";
    const char* hex = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++) {
        buf[9 - i] = hex[v & 0xF];
        v >>= 4;
    }
    vga_print_at(buf, color, x, y);
}

//Converts to string
void u32_to_string(uint32_t value, char* str)
{
    int i = 0;
    int j;
    char temp;

    if (value == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (value > 0)
    {
        str[i++] = '0' + (value % 10);
        value /= 10;
    }

    str[i] = '\0';

    for (j = 0; j < i / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

void vga_print_uint_at(uint32_t value, uint8_t color, int x, int y)
{
    char buffer[16];
    u32_to_string(value, buffer);
    vga_print_at(buffer, color, x, y);
}

//Typing/Printing logic
void vga_put_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        vga_update_hw_cursor();
        return;
    }

    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_putc_at(' ', cursor_color, cursor_x, cursor_y);
        }
        vga_update_hw_cursor();
        return;
    }

    vga_putc_at(c, cursor_color, cursor_x, cursor_y);
    cursor_x++;

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= 25) {
        cursor_y = 24;
    }
    vga_update_hw_cursor();
}

void vga_set_cursor_pos(int x, int y) {
    cursor_x = x;
    cursor_y = y;
    vga_update_hw_cursor();
}

//Clear screen function.
void vga_clear(uint8_t color) {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            vga_putc_at(' ', color, x, y);
        }
    }
}
