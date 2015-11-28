#!/bin/bash
find . | cpio -o -Hnewc |gzip -9 > /mnt/opensource/kernel/rootfs.img
