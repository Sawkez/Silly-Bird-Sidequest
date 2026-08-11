#! /bin/bash

if [ "$#" -lt 1 ]; then
echo "Usage: \$0 <platform>"
echo 'Platforms: linux, windows, win32, psp, web, android, haiku'
exit 1
fi

SCRIPT_DIR=$(dirname "$0")
SRC_DIR=$SCRIPT_DIR/..
PLATFORM=$1

GHCR_IMAGE=ghcr.io/sawkez/sbsidequest-builder-haiku:latest

if [ "$PLATFORM" = "haiku" ]; then
    echo "building the haiku docker image takes like an hour"
    echo "would you like to:"
    echo
    echo "1) build it anyway"
    echo "2) download a prebuilt image instead"
    echo "3) quit"
    echo
    read -rp "choose an option [1-3]: " HAIKU_CHOICE

    case "$HAIKU_CHOICE" in
        1)
            docker build -t sbsidequest-builder-haiku -f $SRC_DIR/docker/haiku/Dockerfile $SRC_DIR/docker/haiku
            ;;
        2)
            docker pull $GHCR_IMAGE || exit 1
            docker tag $GHCR_IMAGE sbsidequest-builder-haiku
            ;;
        *)
            echo "aborted"
            exit 1
            ;;
    esac

    exit 0
fi

docker build -t sbsidequest-builder-$PLATFORM -f $SRC_DIR/docker/dockerfile.$PLATFORM $SRC_DIR/docker