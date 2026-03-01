#!/usr/bin/env bash
set -euo pipefail

# Run from repo root
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

mkdir -p build
mkdir -p iso/boot/grub

echo "[0/6] Cleaning old build artifacts"
rm -f build/*.o build/kernel.elf build/myuos.iso
rm -f iso/boot/kernel.elf

echo "[1/5] Assembling boot.s"
nasm -f elf32 src/boot.s -o build/boot.o

echo "[4/6] Compiling isr.c -> build/isr_c.o"
gcc -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs \
  -c src/isr.c -o build/isr_c.o

echo "[5/6] Compiling kernel.c -> build/kernel.o (debug)"
gcc -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs \
  -g -O0 -fno-omit-frame-pointer \
  -c src/kernel.c -o build/kernel.o

echo "[6/6] Linking kernel.elf"
ld -m elf_i386 -T src/linker.ld -o build/kernel.elf \
  build/boot.o build/isr.o build/idt.o build/kernel.o build/isr_c.o

  -c src/kernel.c -o build/kernel.o

echo "Copying kernel into ISO tree + building ISO"
cp build/kernel.elf iso/boot/kernel.elf
grub-mkrescue -o build/myuos.iso iso >/dev/null 2>&1 || grub-mkrescue -o build/myuos.iso iso


echo "Built: build/myuos.iso"

