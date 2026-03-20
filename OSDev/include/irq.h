#pragma once
#include "isr.h"

void irq_install(void);
void irq_handler(regs_t* r);
