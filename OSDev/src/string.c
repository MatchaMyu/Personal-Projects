#include <stdint.h>

//contains memcpy, memset functions.

//memcpy
void* memcpy(void* dest, const void* src, uint32_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    for (uint32_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

//memset
void* memset(void* dest, int value, uint32_t n) {
    uint8_t* d = (uint8_t*)dest;

    for (uint32_t i = 0; i < n; i++) {
        d[i] = (uint8_t)value;
    }

    return dest;
}

int memcmp(const void* a, const void* b, uint32_t n) {
    const uint8_t* p1 = (const uint8_t*)a;
    const uint8_t* p2 = (const uint8_t*)b;

    for (uint32_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return (int)p1[i] - (int)p2[i];
        }
    }

    return 0;
}
