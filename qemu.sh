#!/bin/sh
set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) \
    -cdrom nvramos.iso \
    -m 4G \
    -nographic
    #-serial stdio
    #-s -S
    #-d int,cpu_reset \
