#!/bin/sh

export CC="i586-elf-gcc"
export AS="i586-elf-as"
export LD="i586-elf-gcc"

./clean.sh
$AS boot.s -o boot.o
$CC -c kernel/*.c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$LD -T linker.ld -o JoOS.bin -ffreestanding -O2 -nostdlib *.o -lgcc
