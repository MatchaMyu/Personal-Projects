#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void* memcpy(void* dest, const void* src, uint32_t n);
void* memset(void* dest, int value, uint32_t n);
int memcmp(const void* a, const void* b, uint32_t n);

#endif
