#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "isr.h"

extern volatile uint32_t g_ticks;
extern volatile uint32_t g_seconds;
extern uint32_t g_timer_frequency;

void timer_init(uint32_t frequency);
void timer_callback(regs_t* regs);
void draw_uptime(void);

#endif
