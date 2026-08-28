#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: \$0 <platform> <build_type>"
    echo 'Platforms: linux, windows, win32, psp, ps2, web, android, haiku'
    echo 'Build types: debug, release (default), memorytest'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")

$SCRIPT_DIR/prepare-resources.sh || exit 1
$SCRIPT_DIR/build-docker.sh "$@" || exit 1
$SCRIPT_DIR/export.sh $1 $2 || exit 1