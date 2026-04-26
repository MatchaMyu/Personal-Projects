#include <stdint.h>
#include "paging.h"
#include "heap.h"
#include "vga.h"

#define PAGE_SIZE 4096
#define PAGE_PRESENT 0x1
#define PAGE_RW      0x2

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

void paging_init(void) {
    uint32_t raw = (uint32_t)kmalloc(8192 + 4096);
    uint32_t aligned = ALIGN_UP(raw, 4096);

    uint32_t* page_directory = (uint32_t*)aligned;
    uint32_t* first_page_table = (uint32_t*)(aligned + 4096);

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        first_page_table[i] = 0;
    }

    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    load_page_directory(page_directory);
    enable_paging();

    vga_print("Paging enabled!\n");
}
