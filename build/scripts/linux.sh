#! /bin/bash

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/../..
BUILD_DIR=$SCRIPT_DIR/../build-files/linux
EXPORT_DIR=$SCRIPT_DIR/../export/linux

mkdir -p $BUILD_DIR
cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug
cmake --build $BUILD_DIR --config Debug

mkdir -p $EXPORT_DIR
rm -rf $EXPORT_DIR/*
cp $BUILD_DIR/sbsidequest $EXPORT_DIR/sidequest.sillybird
cp -r $SRC_DIR/content $EXPORT_DIR/
cp -r $SRC_DIR/mods $EXPORT_DIR/