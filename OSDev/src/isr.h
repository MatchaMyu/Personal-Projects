// isr.h
#pragma once
#include <stdint.h>

typedef struct regs {
    uint32_t gs, fs, es, ds;          // pushed manually
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no;                  // vector number
    uint32_t err_code;                // error code (real or fake 0)
    uint32_t eip, cs, eflags;          // pushed by CPU automatically
    uint32_t useresp, ss;              // only meaningful if coming from ring3->ring0
} regs_t;

void isr_common_handler(regs_t* r);
