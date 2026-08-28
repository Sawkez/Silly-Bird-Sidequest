#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: \$0 <platform> <build_type>"
    echo 'Platforms: linux, windows, win32, psp, ps2, web, android, haiku'
    echo 'Build types: debug, release (default)'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
PLATFORM=$1
USER_BUILD_TYPE=${2:-release}

SRC_DIR=$SCRIPT_DIR/..
BUILD_DIR=$SCRIPT_DIR/build-files/$USER_BUILD_TYPE/$PLATFORM
EXPORT_DIR=$SCRIPT_DIR/export/$USER_BUILD_TYPE/$PLATFORM

COPY_RESOURCES=true

mkdir -p $EXPORT_DIR || exit 1
rm -rf $EXPORT_DIR/* || exit 1

case "$PLATFORM" in
    web)
        cp $BUILD_DIR/sbsidequest.js $BUILD_DIR/sbsidequest.wasm $BUILD_DIR/sbsidequest.data $EXPORT_DIR/ || exit 1
        cp $BUILD_DIR/sbsidequest.html $EXPORT_DIR/index.html || exit 1
        COPY_RESOURCES=false
    ;;

    psp)
        if [ "$USER_BUILD_TYPE" = "debug" ]; then
            cp $BUILD_DIR/sbsidequest.prx $BUILD_DIR/sbsidequest $EXPORT_DIR/ || exit 1
        else
            cp $BUILD_DIR/EBOOT.PBP $EXPORT_DIR || exit 1
        fi
    ;;

    ps2)
        cp $BUILD_DIR/sbsidequest $EXPORT_DIR/sbsidequest.elf || exit 1
    ;;

    linux | haiku)
        cp $BUILD_DIR/sbsidequest $EXPORT_DIR/sidequest.sillybird || exit 1
    ;;

    windows | win32)
        cp $BUILD_DIR/sbsidequest.exe $EXPORT_DIR || exit 1
    ;;

    android)
        cp $SRC_DIR/android-project/app/build/outputs/apk/$USER_BUILD_TYPE/*.apk $EXPORT_DIR/ || exit 1
        COPY_RESOURCES=false
    ;;

    *)
        echo "🧟 Whatever platform that is, either you spelled it wrong or i don't support it (yet)"
        exit 1
    ;;
esac

if [ "$COPY_RESOURCES" = true ]; then
    cp -r $SCRIPT_DIR/build-files/include-files/* $EXPORT_DIR/ || exit 1
    cp -r $SRC_DIR/licenses $EXPORT_DIR/licenses || exit 1
fi