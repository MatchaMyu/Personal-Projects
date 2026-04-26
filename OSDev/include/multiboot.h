#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

typedef struct multiboot_info {
    uint32_t flags;

    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t boot_device;
    uint32_t cmdline;

    uint32_t mods_count;
    uint32_t mods_addr;

    union {
        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } aout_sym;
        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } elf_sec;
    } u;

    uint32_t mmap_length;
    uint32_t mmap_addr;

    // there are more fields in the full spec, but this is enough for now
} multiboot_info_t;

typedef struct multiboot_memory_map {
    uint32_t size;       // size of the entry minus this field
    uint64_t addr;       // base address
    uint64_t len;        // length in bytes
    uint32_t type;       // 1 = available RAM, other values reserved/etc
} __attribute__((packed)) multiboot_memory_map_t;

#endif
