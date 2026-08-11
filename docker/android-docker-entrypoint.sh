#!/bin/bash
set -e

MARKER="${ANDROID_SDK_ROOT}/.provisioned-${ANDROID_PLATFORM}-${ANDROID_BUILD_TOOLS}-${ANDROID_NDK_VERSION}-${ANDROID_CMAKE_VERSION}"

if [ ! -f "$MARKER" ]; then
    mkdir -p "${ANDROID_SDK_ROOT}"
    yes | sdkmanager --sdk_root="${ANDROID_SDK_ROOT}" --licenses >/dev/null
    sdkmanager --sdk_root="${ANDROID_SDK_ROOT}" \
        "platform-tools" \
        "platforms;${ANDROID_PLATFORM}" \
        "build-tools;${ANDROID_BUILD_TOOLS}" \
        "ndk;${ANDROID_NDK_VERSION}" \
        "cmake;${ANDROID_CMAKE_VERSION}"
    touch "$MARKER"
fi

exec "$@"