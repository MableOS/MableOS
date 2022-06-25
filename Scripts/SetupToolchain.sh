TOOLS_DIR=$(pwd)/Tools
CACHE_DIR=$TOOLS_DIR/Cache
BUILD_DIR=$TOOLS_DIR/Build
CROSS_DIR=$TOOLS_DIR/Cross

BUILDROOT_VERSION=2022.05
BUILDROOT_PACKAGE=buildroot-$BUILDROOT_VERSION
BUILDROOT_ARCHIVE=$BUILDROOT_PACKAGE.tar.gz

BUILDROOT_OUTPUT_VERSION=i686-mableos-linux-gnu
BUILDROOT_OUTPUT_PACKAGE="$BUILDROOT_OUTPUT_VERSION"_sdk-buildroot
BUILDROOT_OUTPUT_ARCHIVE=$BUILDROOT_OUTPUT_PACKAGE.tar.gz

MIRROR=https://buildroot.org/downloads

mkdir -p "$CACHE_DIR"
cd "$CACHE_DIR" || exit
wget -nc $MIRROR/$BUILDROOT_ARCHIVE

mkdir -p "$BUILD_DIR"
if [ ! -d "$BUILD_DIR/$BUILDROOT_PACKAGE" ]; then
  cd "$BUILD_DIR" || exit
  tar -xf "$CACHE_DIR/$BUILDROOT_ARCHIVE"
fi

cd "$BUILD_DIR/$BUILDROOT_PACKAGE" || exit
cp "$SOURCE_DIR/Scripts/Configs/Toolchain.config" .config
make source
make sdk

mkdir -p "$SYSROOT_DIR/boot"
cp "$BUILD_DIR/$BUILDROOT_PACKAGE/output/images/bzImage" "$SYSROOT_DIR/boot"

cd "$BUILD_DIR" || exit
tar -xf "$BUILD_DIR/$BUILDROOT_PACKAGE/output/images/$BUILDROOT_OUTPUT_ARCHIVE"

rsync -aH --inplace "$BUILD_DIR/$BUILDROOT_OUTPUT_PACKAGE/" "$CROSS_DIR/"
rsync -aH --inplace "$BUILD_DIR/$BUILDROOT_OUTPUT_PACKAGE/$BUILDROOT_OUTPUT_VERSION/sysroot/" "$SYSROOT_DIR/"

cd "$CROSS_DIR" || exit
./relocate-sdk.sh