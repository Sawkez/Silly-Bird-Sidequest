#! /bin/bash

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/../..
BUILD_DIR=$SCRIPT_DIR/../build-files/linux
EXPORT_DIR=$SCRIPT_DIR/../export/linux

mkdir -p $BUILD_DIR || exot 1
cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug || exit 1
cmake --build $BUILD_DIR --config Debug || exit 1

mkdir -p $EXPORT_DIR || exit 1
rm -rf $EXPORT_DIR/* || exit 1
cp $BUILD_DIR/sbsidequest $EXPORT_DIR/sidequest.sillybird || exit 1
cp -r $SRC_DIR/content $EXPORT_DIR/ || exit 1
cp -r $SRC_DIR/mods $EXPORT_DIR/ || exit 1