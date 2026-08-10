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

DOCKER_ARGS=(--rm -v "$PWD":"$PWD":Z -w "$PWD")

if [ "$PLATFORM" = "android" ]; then
    DOCKER_ARGS+=(
        -v "sbsidequest-gradle-cache:/opt/gradle-cache"
        -v "sbsidequest-android-sdk:/opt/android-sdk"
    )
fi

docker run "${DOCKER_ARGS[@]}" sbsidequest-builder-$PLATFORM bash build/build.sh $PLATFORM $USER_BUILD_TYPE