#!/bin/bash

SCRIPT_PATH=$(dirname "$(readlink -f "$0")")
BUILD_FILES=$SCRIPT_PATH/build-files
INCLUDE_FILES=$SCRIPT_PATH/../include-files
INCLUDE_ARCHIVES=$SCRIPT_PATH/../include-archives
CONTENT=$BUILD_FILES/include-files/content

echo "Copying resources..."
mkdir -p $BUILD_FILES/include-files || exit 1
rm -rf $BUILD_FILES/include-files/* || exit 1
cp -r $INCLUDE_FILES $BUILD_FILES || exit 1

echo "Compressing sidequest.sbsq..."
cd $INCLUDE_ARCHIVES/sidequest || exit 1
zip -0 -r $CONTENT/sidequest.sbsq * || exit 1

echo "Compressing sidequest-hidden.sbsq..."
cd $INCLUDE_ARCHIVES/sidequest-hidden || exit 1
zip -0 -r $CONTENT/sidequest-hidden.sbsq * || exit 1
