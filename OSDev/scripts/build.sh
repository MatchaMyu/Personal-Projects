#!/usr/bin/env bash
set -euo pipefail

# Run from repo root
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

mkdir -p build
mkdir -p iso/boot/grub

echo "[1/5] Assembling boot.s"
nasm -f elf32 src/boot.s -o build/boot.o

echo "[2/5] Compiling kernel.c"
gcc -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs \
  -c src/kernel.c -o build/kernel.o

echo "[3/5] Linking kernel.elf"
ld -m elf_i386 -T src/linker.ld -o build/kernel.elf build/boot.o build/kernel.o

echo "[4/5] Copying kernel into ISO tree"
cp build/kernel.elf iso/boot/kernel.elf

echo "[5/5] Building ISO"
grub-mkrescue -o build/myuos.iso iso >/dev/null 2>&1 || grub-mkrescue -o build/myuos.iso iso

echo "Built: build/myuos.iso"

