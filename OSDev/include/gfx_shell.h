// gfx_shell.h

#ifndef GFX_SHELL_H
#define GFX_SHELL_H
#include <stdint.h>

void gfx_shell_init(void);
void gfx_shell_putchar(char c);
void gfx_shell_print(const char* s);
void gfx_shell_input_char(char c);
void gfx_shell_print_uint(uint32_t value);
void gfx_shell_backspace(void);
void gfx_shell_execute(void);
void gfx_update_hw_cursor(void);

#endif
