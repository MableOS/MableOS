[ -z "$MABLEOS_QEMU_BIN" ] && MABLEOS_QEMU_BIN="qemu-system-i386"
[ -z "$MABLEOS_QEMU_KERNEL" ] && MABLEOS_QEMU_KERNEL="Sysroot/boot/bzImage"
[ -z "$MABLEOS_QEMU_IMAGE" ] && MABLEOS_QEMU_IMAGE="QEMU.img"
[ -z "$MABLEOS_QEMU_ARGS" ] && MABLEOS_QEMU_ARGS="
    -kernel $MABLEOS_QEMU_KERNEL
    -serial stdio
    -vga std
    -drive file=$MABLEOS_QEMU_IMAGE,index=0,media=disk,format=raw"
[ -z "$MABLEOS_QEMU_APP_ARGS" ] && MABLEOS_QEMU_APP_ARGS="
    root=/dev/sda
    console=ttyS0
    init=/bin/SystemService
    vga=0x341"

[ -e /dev/kvm ] && [ -r /dev/kvm ] && [ -w /dev/kvm ] && MABLEOS_QEMU_VIRT="-enable-kvm"

# shellcheck disable=SC2086
$MABLEOS_QEMU_BIN --append "$MABLEOS_QEMU_APP_ARGS" $MABLEOS_QEMU_ARGS $MABLEOS_QEMU_VIRT