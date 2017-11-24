#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) \
    -cdrom nvramos.iso \
    -m 4G \
    -nographic \
    -d int,cpu_reset
    #-s -S
