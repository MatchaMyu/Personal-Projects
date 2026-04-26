#include "commands.h"
#include "vga.h"
#include "shell.h"
#include "isr.h"
#include "memory.h"
#include "heap.h"

extern int shell_row;

int strcmp(const char *a, const char *b) { //Technically compares wrong.
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == *b);
}

//Command list
void cmd_version(void) {
    shell_print("EquineOS v0.01");
}

void cmd_clear(void) {
    shell_clear();
}

void cmd_echo(char *args) {
    shell_print(args);
}

//Heap Test Command
static void cmd_heaptest(void) {
    vga_print("=== HEAP TEST START ===\n");

    void* a = kmalloc(16);
    void* b = kmalloc(32);
    void* c = kmalloc(64);
    void* d = kmalloc(16);
    void* e = 0;

    char buf[11];

    if (!a || !b || !c) {
        vga_print("Heap allocation failed.\n");
        vga_print("=== HEAP TEST FAILED ===\n");
        return;
    }

    vga_print("Allocated blocks:\n");


    vga_print("  a = ");
    vga_print_hex32_cursor((uint32_t)a);
    vga_print("\n");

    vga_print("  b = ");
    vga_print_hex32_cursor((uint32_t)b);
    vga_print("\n");

    vga_print("  c = ");
    vga_print_hex32_cursor((uint32_t)c);
    vga_print("\n");

    vga_print("  d = ");
    vga_print_hex32_cursor((uint32_t)d);
    vga_print("\n");

    uint8_t* pa = (uint8_t*)a;
    uint8_t* pb = (uint8_t*)b;
    uint8_t* pc = (uint8_t*)c;

    for (int i = 0; i < 16; i++) pa[i] = 0xAA;
    for (int i = 0; i < 32; i++) pb[i] = 0xBB;
    for (int i = 0; i < 64; i++) pc[i] = 0xCC;

    int ok = 1;

    for (int i = 0; i < 16; i++) {
        if (pa[i] != 0xAA) ok = 0;
    }
    for (int i = 0; i < 32; i++) {
        if (pb[i] != 0xBB) ok = 0;
    }
    for (int i = 0; i < 64; i++) {
        if (pc[i] != 0xCC) ok = 0;
    }

    if (ok) {
        vga_print("Pattern verification passed.\n");
    } else {
        vga_print("Pattern verification FAILED.\n");
    }

    kfree(b);
    vga_print("Freed block B.\n");

    e = kmalloc(24);

    vga_print("Allocated e = ");
    vga_print_hex32_cursor((uint32_t)e);
    vga_print("\n");

    uint8_t* pe = (uint8_t*)e;
    for (int i = 0; i < 24; i++) {
    pe[i] = 0xDD;
		}

    int a_ok = 1;
    for (int i = 0; i < 16; i++) {
    if (pa[i] != 0xAA) a_ok = 0;
	}

    int c_ok = 1;
    for (int i = 0; i < 64; i++) {
    if (pc[i] != 0xCC) c_ok = 0;
	}

    if (a_ok && c_ok) {
    	vga_print("Neighbor integrity passed.\n");
    } else {
    	vga_print("Neighbor integrity FAILED.\n");
        if (!a_ok) vga_print("  A corrupted\n");
        if (!c_ok) vga_print("  C corrupted\n");
	}

    vga_print("=== HEAP TEST END ===\n");
}


//Process command list
void process_command(char *input) {
    char *args = input;

    while (*args && *args != ' ') {
        args++;
    }

    if (*args == ' ') {
        *args = '\0';
        args++;
    }

    while (*args == ' ') {
        args++;
    }

    if (strcmp(input, "version")) {
        cmd_version();
    } 
    else if (strcmp(input, "echo")) {
        cmd_echo(args);
    }
    else if (strcmp(input, "clear")) {
        cmd_clear();
    } 
    else if (strcmp(input, "heap")) {
    cmd_heaptest();
    return; 
    }
    else if (strcmp(input, "mem")) {
    mem_stats_t stats;
    char buf[32];

    mem_get_stats(&stats);

    shell_print("Heap stats:\n");

    shell_print("Used bytes: ");
    vga_print_uint(stats.used_bytes);
    vga_print("\n");

    shell_print("Free bytes: ");
    vga_print_uint(stats.free_bytes);
    vga_print("\n");

    shell_print("Blocks: ");
    vga_print_uint(stats.block_count);
    vga_print("\n");

    shell_print("Free blocks: ");
    vga_print_uint(stats.free_block_count);
    vga_print("\n");

    shell_print("Largest free block: ");
    vga_print_uint(stats.largest_free_block);
    vga_print("\n");

    } else if (strcmp(input, "heap")) {
    char buf[32];
    uint32_t start = heap_get_start();
    uint32_t end   = heap_get_end();
    uint32_t limit = heap_get_limit();

    shell_print("Heap info:\n");

    shell_print("Start: ");
    vga_print_hex32_cursor(start);
    shell_print("\n");

    shell_print("End: ");
    vga_print_hex32_cursor(end);
    shell_print("\n");

    shell_print("Limit: ");
    vga_print_hex32_cursor(limit);
    shell_print("\n");

    shell_print("Consumed bytes: ");
    vga_print_uint(end - start);
    shell_print("\n");

    shell_print("Remaining bytes: ");
    vga_print_uint(limit - end);
    shell_print("\n");

    return;
    }//Panic command
     else if (strcmp(input, "panic")) {
	panic("This should not print", 0);
	return;
	}
    //Invalid command.
    else {
        shell_print("Neigh, that is not a valid command.");
    }

}
//end process command list.
