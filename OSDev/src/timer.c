#include <stdint.h>
#include "ports.h"
#include "isr.h"
#include "kernel.h"
#include "timer.h"
#include "vga.h"

#define PIT_COMMAND  0x43
#define PIT_CHANNEL0 0x40
#define PIT_BASE_FREQUENCY 1193182

volatile uint32_t g_ticks = 0;
volatile uint32_t g_seconds = 0;
uint32_t g_timer_frequency = 100;

void timer_callback(regs_t* regs)
{
    g_ticks++;

    if ((g_ticks % g_timer_frequency) == 0)
    {
        g_seconds++;
	draw_uptime();
    }
}

void timer_init(uint32_t frequency)
{
    g_timer_frequency = frequency;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);

    register_interrupt_handler(32, timer_callback);
}

void draw_uptime(void)
{
    uint32_t hours   = g_seconds / 3600;
    uint32_t minutes = (g_seconds % 3600) / 60;
    uint32_t seconds = g_seconds % 60;

    vga_print_at("Uptime: ", 0x0F, 0, 0);

    vga_print_uint_at(hours, 0x0F, 8, 0);
    vga_print_at(":", 0x0F, 10, 0);

    vga_print_uint_at(minutes, 0x0F, 11, 0);
    vga_print_at(":", 0x0F, 13, 0);

    vga_print_uint_at(seconds, 0x0F, 14, 0);
}
