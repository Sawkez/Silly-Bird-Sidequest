#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: \$0 <platform> <build_type>"
    echo 'Platforms: linux, windows, win32, psp, web, android, haiku'
    echo 'Build types: debug, release (default)'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
PLATFORM=$1
USER_BUILD_TYPE=${2:-release}

docker run --rm -v "$PWD":"$PWD":Z -w "$PWD" sbsidequest-builder-$PLATFORM bash build/build.sh $PLATFORM $USER_BUILD_TYPE