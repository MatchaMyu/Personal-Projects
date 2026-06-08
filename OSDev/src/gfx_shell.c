// gfx_shell.c
//the commands.c file takes BOTH this and VGA.
#include "gfx_shell.h"
#include "graphics.h"
#include "font8x8.h"
#include "string.h"
#include "shell/commands.h"
#include "shell/console.h"
#include "keyboard.h"
#include "vga.h"

#define SHELL_X 16
#define SHELL_Y 32
#define SHELL_W 608
#define SHELL_H 400

#define CHAR_W 8
#define CHAR_H 8

#define MAX_INPUT 128

static int cursor_x = SHELL_X;
static int cursor_y = SHELL_Y;

static char input[MAX_INPUT];
static int input_len = 0;

static void gfx_shell_draw_rect(int x, int y, int w, int h, uint32_t color) {
    gfx_fill_rect(x, y, w, 1, color);           // top
    gfx_fill_rect(x, y + h - 1, w, 1, color);   // bottom
    gfx_fill_rect(x, y, 1, h, color);           // left
    gfx_fill_rect(x + w - 1, y, 1, h, color);   // right
}

void gfx_shell_putchar(char c) {
    if (c == '\n') {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;
        return;
    }

    if (c == '\b') {
        gfx_shell_backspace();
        return;
    }

    gfx_draw_char(cursor_x, cursor_y, c, 0xFFFFFF);
    cursor_x += CHAR_W;

    if (cursor_x >= SHELL_X + SHELL_W - CHAR_W) {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;
    }
}

void gfx_shell_print(const char* str) {
    if (!str) return;
    while (*str) {
        gfx_shell_putchar(*str++);
    }
}

void gfx_shell_print_hex32(uint32_t value) {
    const char* hex = "0123456789ABCDEF";

    gfx_shell_putchar('0');
    gfx_shell_putchar('x');

    for (int shift = 28; shift >= 0; shift -= 4) {
        gfx_shell_putchar(hex[(value >> shift) & 0xF]);
    }
}

void gfx_shell_init(void) {
    gfx_clear(0x202020);

    gfx_fill_rect(8, 8, 624, 464, 0x303030);
    gfx_shell_draw_rect(8, 8, 624, 464, 0xFFFFFF);

    cursor_x = SHELL_X;
    cursor_y = SHELL_Y;

    gfx_shell_print("EquineOS Graphical Shell\n");
    gfx_shell_print("> ");
}


void gfx_shell_input_char(char c) {
    if (c == '\n') {
        cursor_x = SHELL_X;
        cursor_y += CHAR_H;

        input[input_len] = '\0';
        process_command(input);

        input_len = 0;
        input[0] = '\0';

        gfx_shell_print("> ");
        return;
    }

    if (c == '\b') {
        gfx_shell_backspace();
        return;
    }

    if (input_len < MAX_INPUT - 1) {
        input[input_len++] = c;
        gfx_shell_putchar(c);
    }
}

void gfx_shell_print_uint(uint32_t value)
{
    char buffer[16];
    u32_to_dec_string(value, buffer);
    gfx_shell_print(buffer);
}

void gfx_shell_backspace(void) {
    if (input_len <= 0) return;

    input_len--;

    cursor_x -= CHAR_W;
    if (cursor_x < SHELL_X) {
        cursor_x = SHELL_X;
    }

    gfx_fill_rect(cursor_x, cursor_y, CHAR_W, CHAR_H, 0x303030);
}

void gfx_update_hw_cursor(void) {
    uint16_t pos = (uint16_t)(cursor_y * SHELL_W + cursor_x);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

