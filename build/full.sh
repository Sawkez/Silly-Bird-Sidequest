#! /bin/bash

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <platform> [build_type] [extra_cmake_args...]"
    echo 'Platforms: linux, windows, win32, psp, ps2, web, android, haiku'
    echo 'Build types: debug, release (default)'
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
PLATFORM=$1
USER_BUILD_TYPE=${2:-release}

"$SCRIPT_DIR/prepare-resources.sh" || exit 1
"$SCRIPT_DIR/build.sh" "$@" || exit 1
"$SCRIPT_DIR/export.sh" "$PLATFORM" "$USER_BUILD_TYPE" || exit 1