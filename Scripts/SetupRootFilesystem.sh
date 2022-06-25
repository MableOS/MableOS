printf "Copying base system..."
rsync -aH --inplace "$SYSROOT_DIR/" "$MOUNT_DIR/"
rsync -aH --inplace "$SOURCE_DIR/Base/" "$MOUNT_DIR/"
echo "OK"