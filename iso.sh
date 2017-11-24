#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/nvramos.kernel isodir/boot/nvramos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
set default=0
set timeout=1
menuentry "nvramos" {
	multiboot /boot/nvramos.kernel
}
EOF
grub-mkrescue -o nvramos.iso isodir
