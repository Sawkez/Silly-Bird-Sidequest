#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: \$0 <platform>"
    echo 'Platforms: linux, windows, win32, psp, web, android, haiku'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/..
PLATFORM=$1

docker build -t sbsidequest-builder-$1 -f $SRC_DIR/docker/dockerfile.$1 $SRC_DIR/docker