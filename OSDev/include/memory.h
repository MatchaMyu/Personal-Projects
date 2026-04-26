#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "multiboot.h"

void print_multiboot_memory_map(multiboot_info_t* mbi);

#endif
