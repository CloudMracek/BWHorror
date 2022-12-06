#!/bin/bash

rm -rf build
mkdir -p build

cmake -Bbuild \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=${PSN00BSDK_INSTALL_DIR}/lib/libpsn00b/cmake/sdk.cmake \
    -DPSN00BSDK_TC="" \
    -DPSN00BSDK_TARGET="mipsel-none-elf" \
    .

exec make ${1:-all} -C build
