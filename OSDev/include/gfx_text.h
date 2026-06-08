#ifndef GFX_TEXT_H
#define GFX_TEXT_H

//Tied to graphics.c right now. Can split them later.

#include <stdint.h>

void gfx_draw_char(int x, int y, char c, uint32_t color);
void gfx_draw_string(int x, int y, const char *s, uint32_t color);
void gfx_draw_string_scaled(int x, int y, const char *s, uint32_t color, int scale);
void gfx_draw_char_scaled(int x, int y, char c, uint32_t color, int scale);

#endif
