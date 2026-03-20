# MyuOS Makefile

ASM      = nasm
CC       = gcc
LD       = ld
GRUBMK   = grub-mkrescue

CFLAGS   = -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs -Iinclude
KFLAGS   = $(CFLAGS) -g -O0 -fno-omit-frame-pointer
ASFLAGS  = -f elf32
LDFLAGS  = -m elf_i386 -T linker/linker.ld

ISO_DIR  = iso
BOOT_DIR = $(ISO_DIR)/boot
BUILD_DIR = build
KERNEL   = $(BUILD_DIR)/kernel.elf
ISO_OUT  = $(BUILD_DIR)/myuos.iso

OBJS = $(BUILD_DIR)/boot.o \
       $(BUILD_DIR)/isr.o \
       $(BUILD_DIR)/vga.o \
       $(BUILD_DIR)/idt.o \
       $(BUILD_DIR)/kernel.o \
       $(BUILD_DIR)/isr_c.o \
       $(BUILD_DIR)/irq.o \
       $(BUILD_DIR)/pic.o \
       $(BUILD_DIR)/ports.o \
       $(BUILD_DIR)/timer.o \
       $(BUILD_DIR)/keyboard.o \
       $(BUILD_DIR)/shell.o

all: $(KERNEL) $(ISO_OUT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link kernel
$(KERNEL): $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Build ISO
$(ISO_OUT): $(KERNEL) | $(BUILD_DIR)
	cp $(KERNEL) $(BOOT_DIR)/kernel.elf
	$(GRUBMK) -o $@ $(ISO_DIR)

# Assembly sources
$(BUILD_DIR)/boot.o: src/boot.s | $(BUILD_DIR)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/isr.o: src/isr.s | $(BUILD_DIR)
	$(ASM) $(ASFLAGS) $< -o $@

#Shell
$(BUILD_DIR)/shell.o: src/shell/shell.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# C sources
$(BUILD_DIR)/kernel.o: src/kernel.c | $(BUILD_DIR)
	$(CC) $(KFLAGS) -c $< -o $@

$(BUILD_DIR)/idt.o: src/idt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/isr_c.o: src/isr.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/irq.o: src/irq.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pic.o: src/pic.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ports.o: src/ports.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/timer.o: src/timer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: src/keyboard.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: src/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# Rebuild from scratch
re: clean all

.PHONY: all clean re

#Run
run: $(ISO_OUT)
	qemu-system-i386 -m 256M -cdrom $(ISO_OUT) -boot d -no-reboot -no-shutdown -display curses
