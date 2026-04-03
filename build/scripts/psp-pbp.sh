#! /bin/bash

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/../..
BUILD_DIR=$SCRIPT_DIR/../build-files/psp-pbp
EXPORT_DIR=$SCRIPT_DIR/../export/psp-pbp

mkdir -p $BUILD_DIR
psp-cmake -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug
cmake --build $BUILD_DIR

mkdir -p $EXPORT_DIR
rm -rf $EXPORT_DIR/*
cp $BUILD_DIR/EBOOT.PBP $EXPORT_DIR
cp -r $SRC_DIR/content $EXPORT_DIR/
cp -r $SRC_DIR/mods $EXPORT_DIR/