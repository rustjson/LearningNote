http://mgalgs.github.io/2012/03/23/how-to-build-a-custom-linux-kernel-for-qemu.html

cd to busybox && make && make install

touch _install/init

Here's the contents of my /init:
```
#!/bin/sh

/bin/mount -t proc none /proc
/bin/mount -t sysfs sysfs /sys
/bin/sh
```

```
chmod 755 _install/init
```

make init:
```
find . -print0 | cpio --null -ov --format=newc > ../my-inits.cpio
```

## create disk

```
qemu-img create disk.img 512M
mkfs.ext4 -F disk.img
```

# Start

qemu-system-$(ARCH) -hda disk.img -kernel linuxsource/arch/x86/boot/bzImage -initrd my-inits.cpio






