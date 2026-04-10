#! /bin/bash

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/../..
BUILD_DIR=$SCRIPT_DIR/../build-files/psp-prx
EXPORT_DIR=$SCRIPT_DIR/../export/psp-prx

mkdir -p $BUILD_DIR || exit 1
psp-cmake -S $SRC_DIR -B $BUILD_DIR -DBUILD_PRX=1 -DCMAKE_BUILD_TYPE=Debug || exit 1
cmake --build $BUILD_DIR || exit 1

mkdir -p $EXPORT_DIR || exit 1
rm -rf $EXPORT_DIR/* || exit 1
cp $BUILD_DIR/sbsidequest.prx $EXPORT_DIR || exit 1
cp $BUILD_DIR/sbsidequest $EXPORT_DIR || exit 1
cp -r $SRC_DIR/content $EXPORT_DIR/ || exit 1
cp -r $SRC_DIR/mods $EXPORT_DIR/ || exit 1
