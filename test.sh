#!/bin/sh

./build.sh
qemu-system-i386 -kernel JoOS.bin
