#!/bin/bash

mkdir -p isodir

mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp Aqeous.bin isodir/boot/Aqeous.bin
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Aqeous" {
	multiboot /boot/Aqeous.bin
	boot
}
EOF
