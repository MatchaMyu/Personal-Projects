Tiny Test Programs.
Knowing little about assembly and C, I'll create a simple program in debian.

nano hello.c

This will create a "hello.c" program that's currently blank. The code is input below:

#include <stdio.h>

int main() {
    printf("Toolchain works!\n");
    return 0;
}

compile:
gcc hello.c -o hello
./hello

This gives the response "Toolchain works!"

Now in preperation for creating the OS...

sudo apt install -y grub-pc-bin xorriso

Then create the directory...

And finally, all basic commands.
boot.s
kernel.c
grub.cfg
linker.ld

The code is in the respective repository.

Then, finally...

nasm -f elf32 boot.s -o boot.o
gcc -m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs -c kernel.c -o kernel.o
ld -m elf_i386 -T linker.ld -o kernel.elf boot.o kernel.o
cp kernel.elf iso/boot/kernel.elf
grub-mkrescue -o myuos.iso iso

This builds them into one ISO.

EXECUTING THE ISO:

Executing it is a little more complex. Debian is text based. The kernel prints to VGA text memory which requires a graphical display. It will show nothing on debian in -nographic unless we rewrite the kernel to print in serial.

So MOVE the ISO to the primary computer and boot it from there using QEMU! This launches a VM for it.

(In the right folder)
qemu-system-x86_64 -m 256M -cdrom myuos.iso -boot d -no-reboot -monitor none

