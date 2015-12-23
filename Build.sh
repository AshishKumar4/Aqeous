#!/bin/sh

rm -rfv isodir
rm -rfv objs
make all
mkdir -p isodir

mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp initrd.img isodir/boot/initrd.img
cp objs/Aqeous isodir/boot/Aqeous
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Aqeous" {
	multiboot /boot/Aqeous
	module /boot/initrd.img
}
EOF
grub-mkrescue -o aqeous.iso isodir
