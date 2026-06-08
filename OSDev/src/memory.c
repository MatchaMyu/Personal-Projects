#include <stdint.h>
#include "multiboot.h"
#include "vga.h"
#include "memory.h"
#include "heap.h"

#define MIN_SPLIT_SIZE 8 //Defines minimum split size
#define ALIGN8(x) (((x) + 7) & ~7)
#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

extern uint32_t end;
int heap_found = 0;
uint32_t chosen_heap_start = 0;
uint32_t chosen_heap_limit = 0;

static void print_hex32(uint32_t value) {
    vga_print_hex32_cursor(value);
}

static void print_hex64(uint64_t value) {
    uint32_t hi = (uint32_t)(value >> 32);
    uint32_t lo = (uint32_t)(value & 0xFFFFFFFF);

    if (hi != 0) {
        vga_print_hex32_cursor(hi);
        vga_print_hex32_cursor(lo);
    } else {
        vga_print_hex32_cursor(lo);
    }
}

static const char* mmap_type_name(uint32_t type) {
    switch (type) {
        case 1: return "AVAILABLE";
        case 2: return "RESERVED";
        case 3: return "ACPI_RECLAIM";
        case 4: return "ACPI_NVS";
        case 5: return "BADRAM";
        default: return "UNKNOWN";
    }
}

void print_multiboot_memory_map(multiboot_info_t* mbi) { //memory_init...
    uint32_t kernel_end = ((uint32_t)&end + 0xFFF) & ~0xFFF;

    vga_print("=== Multiboot Memory Map ===\n");

    if (!(mbi->flags & (1 << 6))) {
        vga_print("No memory map provided by bootloader.\n");
        return;
    }

    vga_print("mmap_addr: ");
    print_hex32(mbi->mmap_addr);
    vga_print("\n");

    vga_print("mmap_length: ");
    print_hex32(mbi->mmap_length);

    uintptr_t mmap_end = mbi->mmap_addr + mbi->mmap_length;
    multiboot_memory_map_t* entry =
        (multiboot_memory_map_t*)(uintptr_t)mbi->mmap_addr;

    uint64_t total_usable = 0;

    while ((uintptr_t)entry < mmap_end) {
        uint64_t base = entry->addr;
        uint64_t length = entry->len;
        uint64_t region_end = base + length;

        vga_print("base=");
        print_hex64(entry->addr);

        vga_print(" len=");
        print_hex64(entry->len);

        vga_print(" type=");
        print_hex32(entry->type);

        vga_print(" ");
        vga_print(mmap_type_name(entry->type));
        vga_print("\n");

        if (entry->type == 1) {
            total_usable += length;

	    uint32_t aligned_start = ALIGN_UP(kernel_end, 4096);

            if (!heap_found &&
                (uint64_t)kernel_end >= base &&
                (uint64_t)kernel_end < region_end) {
                chosen_heap_start = aligned_start;
                chosen_heap_limit = (uint32_t)region_end;
                heap_found = 1;
            }
        }

        entry = (multiboot_memory_map_t*)
            ((uintptr_t)entry + entry->size + sizeof(entry->size));
    }

    vga_print("============================\n");
    vga_print("\nTotal usable RAM (bytes): ");
    print_hex64(total_usable);

    uint32_t total_mb = (uint32_t)(total_usable / (1024 * 1024));
    vga_print("\n");
    vga_print("Total usable RAM: ");
    vga_print_uint(total_mb);
    vga_print(" MB\n");

    if (heap_found) {
    	heap_init(chosen_heap_start, chosen_heap_limit);

    	vga_print("Heap start: ");
    	print_hex32(chosen_heap_start);
    	vga_print("\n");

    	vga_print("Heap limit: ");
    	print_hex32(chosen_heap_limit);
    	vga_print("\n");
    } else {
    	vga_print("No suitable heap region found.\n");
	}


}

