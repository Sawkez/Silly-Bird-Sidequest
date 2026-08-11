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

case "$USER_BUILD_TYPE" in
    release)
        BUILD_TYPE=Release
    ;;

    debug)
        BUILD_TYPE=Debug
    ;;

    *)
        echo "Unknkown build type. Use either debug or release"
        exit 1
    ;;
esac

SRC_DIR=$SCRIPT_DIR/..
CMAKE_COMMAND=(cmake)

case "$PLATFORM" in
    web)
        CMAKE_COMMAND=(emcmake cmake)
    ;;

    psp)
        CMAKE_COMMAND=(psp-cmake)
    ;;

    linux)
    ;;
    
    haiku)
    	if [ "$(uname -s)" != "Haiku" ]; then
    		CMAKE_COMMAND+=( "-DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/haiku-x64.cmake" )
    	fi
	;;

    windows)
        MINGW_DIR=/usr/x86_64-w64-mingw32/sys-root/mingw
        export PKG_CONFIG=$MINGW_DIR/lib/pkgconfig
        CMAKE_COMMAND+=( "-DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/mingw-w64.cmake" )
    ;;

    win32)
        MINGW_DIR=/usr/i686-w64-mingw32/sys-root/mingw
        export PKG_CONFIG=$MINGW_DIR/lib/pkgconfig
        CMAKE_COMMAND+=( "-DCMAKE_TOOLCHAIN_FILE=$SRC_DIR/mingw-w32.cmake" )
    ;;

    android)
        cd $SRC_DIR/android-project
        if [ "$USER_BUILD_TYPE" = "debug" ]; then
            ./gradlew assembleDebug || exit 1
        else
            ./gradlew assembleRelease || exit 1
        fi

        exit 0
    ;;

    *)
        echo "🧟 Whatever platform that is, either you spelled it wrong or i don't support it (yet)"
        exit 1
    ;;
esac

BUILD_DIR=$SCRIPT_DIR/build-files/$USER_BUILD_TYPE/$PLATFORM

mkdir -p $BUILD_DIR || exit 1
"${CMAKE_COMMAND[@]}" -G Ninja -S $SRC_DIR -B $BUILD_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE || exit 1
ninja -C $BUILD_DIR || exit 1