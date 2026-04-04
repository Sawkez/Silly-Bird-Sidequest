#! /bin/bash

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/../..
BUILD_DIR=$SCRIPT_DIR/../build-files/windows
EXPORT_DIR=$SCRIPT_DIR/../export/windows
MING64_DIR=/usr/x86_64-w64-mingw32/sys-root/mingw
DLL_DIR=$MING64_DIR/bin

mkdir -p $BUILD_DIR || exit 1
PKG_CONFIG_LIBDIR=$MING64_DIR/lib/pkgconfig cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/mingw-w64.cmake || exit 1
cmake --build $BUILD_DIR --config Debug || exit 1

mkdir -p $EXPORT_DIR || exit 1
rm -rf $EXPORT_DIR/* || exit 1
cp $BUILD_DIR/sbsidequest.exe $EXPORT_DIR/ || exit 1
cp -r $SRC_DIR/content $EXPORT_DIR/ || exit 1
cp -r $SRC_DIR/mods $EXPORT_DIR/ || exit 1
cp -r $DLL_DIR/{SDL2.dll,SDL3.dll,SDL2_image.dll,libgcc_s_seh-1.dll,libtiff-5.dll,libwebp-7.dll,libwebpdemux-2.dll,libjpeg-62.dll,zlib1.dll,libwinpthread-1.dll,libsharpyuv-0.dll} $EXPORT_DIR/ || exit 1