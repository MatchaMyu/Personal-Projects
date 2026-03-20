#include "keyboard.h"
#include "vga.h"
#include "ports.h"
#include <stdint.h>

static int shift_pressed = 0;
static char input_buffer[256];
static int input_pos = 0;
static int input_done = 0;

void keyboard_reset_buffer(void) {
    input_pos = 0;
    input_done = 0;
    input_buffer[0] = '\0';
}

int keyboard_input_ready(void) {
    return input_done;
}

char* keyboard_get_buffer(void) {
    return input_buffer;
}

static const char scancode_to_ascii[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,    // Ctrl
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,    // Left Shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,    // Right Shift
    '*',
    0,    // Alt
    ' ',
    0,    // Caps Lock
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // F1-F10
    0,    // Num Lock
    0,    // Scroll Lock
    0,    // Home
    0,    // Up
    0,    // Page Up
    '-',
    0,    // Left
    0,
    0,    // Right
    '+',
    0,    // End
    0,    // Down
    0,    // Page Down
    0,    // Insert
    0,    // Delete
    0, 0, 0,
    0,    // F11
    0     // F12
};

static char apply_shift(char c)
{
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }

    switch (c) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        case '`': return '~';
        default: return c;
    }
}

void keyboard_handler(void)
{
    uint8_t scancode = inb(0x60);

    // Shift pressed
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }

    // Shift released
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    // Ignore key releases
    if (scancode & 0x80) {
        return;
    }

    if (scancode >= 128) {
        return;
    }

    // Handle Enter
    if (scancode == 0x1C) {
        input_buffer[input_pos] = '\0';
        input_done = 1;
        // optional: move cursor to next line or print newline behavior
        return;
    }

    // Handle Backspace
    if (scancode == 0x0E) {
        if (input_pos > 0) {
            input_pos--;
            input_buffer[input_pos] = '\0';
            vga_put_char('\b');

        }
        return;
    }


    char c = scancode_to_ascii[scancode];

    if (c == 0) {
        return;
    }

    if (shift_pressed) {
        c = apply_shift(c);
    }


    if (input_pos < 255) {
	input_buffer[input_pos++] = c;
	input_buffer[input_pos] = '\0';
	vga_put_char(c);
    }

}
