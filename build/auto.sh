#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: \$0 <platform> <build_type>"
    echo 'Platforms: linux, windows, win32, psp, web, android'
    echo 'Build types: Debug, MinSizeRel (default)'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
PLATFORM=$1
BUILD_TYPE=${2:-MinSizeRel}
BUILD_NAME=$PLATFORM
SRC_DIR=$SCRIPT_DIR/..
CMAKE_COMMAND=(cmake)
COPY_RESOURCES=true

case "$PLATFORM" in
    web)
        CMAKE_COMMAND=(emcmake cmake)
        EXE_IN="sbsidequest.html"
        EXE_OUT="index.html"
        COPY_RESOURCES=false
        if [ "$BUILD_TYPE" = "MinSizeRel" ]; then
            BUILD_TYPE="Release"
        fi
    ;;

    psp)
        if [ "$BUILD_TYPE" = "Debug" ]; then
            BUILD_NAME="psp-prx"
            EXE_IN="sbsidequest.prx"
            EXE_OUT="sbsidequest.prx"

        else
            BUILD_NAME="psp-pbp"
            EXE_IN="EBOOT.PBP"
            EXE_OUT="EBOOT.PBP"
            BUILD_TYPE="Release"
        fi

        CMAKE_COMMAND=(psp-cmake)
    ;;

    linux)
        CMAKE_COMMAND="cmake"
        EXE_IN="sbsidequest"
        EXE_OUT="sidequest.sillybird"
    ;;

    windows)
        MINGW_DIR=/usr/x86_64-w64-mingw32/sys-root/mingw
        export PKG_CONFIG=$MINGW_DIR/lib/pkgconfig
        CMAKE_COMMAND+=( "-DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/mingw-w64.cmake" )
        EXE_IN="sbsidequest.exe"
        EXE_OUT="sbsidequest.exe"
    ;;

    win32)
        MINGW_DIR=/usr/i686-w64-mingw32/sys-root/mingw
        export PKG_CONFIG=$MINGW_DIR/lib/pkgconfig
        CMAKE_COMMAND+=( "-DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/mingw-w32.cmake" )
        EXE_IN="sbsidequest.exe"
        EXE_OUT="sbsidequest.exe"
    ;;

    android)
        cd $SRC_DIR/android-project
        if [ "$BUILD_TYPE" = "Debug" ]; then
            ./gradlew assembleDebug || exit 1
        else
            ./gradlew assembleRelease || exit 1
        fi

        exit 0
    ;;

    *)
        echo "🧟 Whatever platform that is, either you spelled it wrong or i don't support it (yet)"
        exit 1
    ;;
esac

$SCRIPT_DIR/prepare-resources.sh

BUILD_DIR=$SCRIPT_DIR/build-files/$BUILD_NAME
EXPORT_DIR=$SCRIPT_DIR/export/$BUILD_NAME

mkdir -p $BUILD_DIR || exit 1
"${CMAKE_COMMAND[@]}" -G Ninja -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
ninja -C $BUILD_DIR || exit 1

mkdir -p $EXPORT_DIR || exit 1
rm -rf $EXPORT_DIR/* || exit 1
cp $BUILD_DIR/$EXE_IN $EXPORT_DIR/$EXE_OUT || exit 1

if [ "$COPY_RESOURCES" = true ]; then
    cp -r $SCRIPT_DIR/build-files/include-files/* $EXPORT_DIR/ || exit 1
    cp -r $SRC_DIR/licenses $EXPORT_DIR/licenses || exit 1
fi

if [ "$BUILD_NAME" = "psp-prx" ]; then
    cp $BUILD_DIR/sbsidequest $EXPORT_DIR/sbsidequest || exit 1
fi

if [ "$BUILD_NAME" = "web" ]; then
    cp $BUILD_DIR/sbsidequest.js $BUILD_DIR/sbsidequest.wasm $BUILD_DIR/sbsidequest.data $EXPORT_DIR/ || exit 1
fi