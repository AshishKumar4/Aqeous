#!/bin/sh
make all
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp objs/aqeous.water isodir/boot/aqeous.water
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "aqeous.water" {
	multiboot /boot/aqeous.water
}
EOF
grub-mkrescue -o aqeous.iso isodir
