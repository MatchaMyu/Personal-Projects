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

/*
//Block header
typedef struct block_header {
    uint32_t magic;
    uint32_t size;
    uint32_t free;
    struct block_header* next;
} block_header_t;
#define ALLOC_MAGIC 0xC0FFEE42
static block_header_t* heap_head = 0;

//split block
static void split_block(block_header_t* block, uint32_t size) {
    if (block->size < size + sizeof(block_header_t) + MIN_SPLIT_SIZE) {
        return;
    }

    uint32_t new_block_addr =
        (uint32_t)block + sizeof(block_header_t) + size;

    block_header_t* new_block = (block_header_t*)new_block_addr;
    new_block->magic = ALLOC_MAGIC;
    new_block->size = block->size - size - sizeof(block_header_t);
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

//Create block
static block_header_t* create_block(uint32_t size) {
    uint32_t total_size = sizeof(block_header_t) + size;

    if (heap_end > heap_limit) return 0;
    if (total_size > heap_limit - heap_end) return 0;

    block_header_t* block = (block_header_t*)heap_end;

    block->magic = ALLOC_MAGIC;
    block->size = size;
    block->free = 0;
    block->next = 0;

    heap_end += total_size;
    return block;
}

//Free Block
static block_header_t* find_free_block(uint32_t size) {
    block_header_t* current = heap_head;

    while (current) {
        if (current->magic == ALLOC_MAGIC &&
            current->free &&
            current->size >= size) {
            return current;
        }
        current = current->next;
    }

    return 0;
}

//colesce blocks
static void coalesce_free_blocks(void) {
    block_header_t* current = heap_head;

    while (current && current->next) {
        uint32_t current_end =
            (uint32_t)current + sizeof(block_header_t) + current->size;

        if (current->magic == ALLOC_MAGIC &&
            current->next->magic == ALLOC_MAGIC &&
            current->free &&
            current->next->free &&
            current_end == (uint32_t)current->next) {

            block_header_t* next = current->next;

            current->size += sizeof(block_header_t) + next->size;
            current->next = next->next;

        } else {
            current = current->next;
        }
    }
}

//kmalloc
void* kmalloc(uint32_t size) {
    if (size == 0) return 0;

    if (size > 0xFFFFFFFF - 7) return 0;
    uint32_t aligned_size = (size + 7) & ~7;

    block_header_t* block = find_free_block(aligned_size);

    if (block) {
        split_block(block, aligned_size);
        block->free = 0;
        return (void*)((uint32_t)block + sizeof(block_header_t));
    }

    block = create_block(aligned_size);
    if (!block) {
        return 0;
    }

    if (!heap_head) {
        heap_head = block;
    } else {
        block_header_t* current = heap_head;
        while (current->next) {
            current = current->next;
        }
        current->next = block;
    }

    return (void*)((uint32_t)block + sizeof(block_header_t));
}

//kfree
void kfree(void* ptr) {
    if (!ptr) return;

    block_header_t* block =
        (block_header_t*)((uint32_t)ptr - sizeof(block_header_t));

    if (block->magic != ALLOC_MAGIC) {
        return; // or panic
    }

    if (block->free) {
        return; // double free
    }

    block->free = 1;
    coalesce_free_blocks();
}

//for the MEM command used in commands.c
void mem_get_stats(mem_stats_t* stats) {
    if (!stats) {
        return;
    }

    stats->used_bytes = 0;
    stats->free_bytes = 0;
    stats->block_count = 0;
    stats->free_block_count = 0;
    stats->largest_free_block = 0;

    block_header_t* current = heap_head;

    while (current) {
        if (current->magic != ALLOC_MAGIC) {
            return;
        }

        stats->block_count++;

        if (current->free) {
            stats->free_bytes += current->size;
            stats->free_block_count++;

            if (current->size > stats->largest_free_block) {
                stats->largest_free_block = current->size;
            }
        } else {
            stats->used_bytes += current->size;
        }

        current = current->next;
    }
} */
