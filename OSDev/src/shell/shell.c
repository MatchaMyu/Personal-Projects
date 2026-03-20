#include "shell.h"
#include "vga.h"
#include "keyboard.h"

void draw_uptime(void);

void process_command(char *input);

static int shell_row = 10; //This will need to be changed

int strcmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == *b);
}

void shell_run(void) {
    while (1) {
        keyboard_reset_buffer();

        vga_print_at("> ", 0x0F, 0, shell_row);
        vga_set_cursor_pos(2, shell_row);

        while (!keyboard_input_ready())  {
	__asm__ __volatile__("hlt");
        }

	process_command(keyboard_get_buffer());

	shell_row += 2;

        if (shell_row >= 22) {
            shell_row = 10;   // temporary wraparound
        }

    }
}

void process_command(char *input) {
    if (strcmp(input, "version")) {
        vga_print_at("EquineOS v0.01", 0x0F, 0, shell_row + 1);
    } else {
        vga_print_at("Neigh, that is not a valid command.", 0x0F, 0, shell_row + 1);
    }
}
