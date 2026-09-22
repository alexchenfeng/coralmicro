#!/bin/bash
# Copyright 2026 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
LIB_ROOT="$(cd "$SCRIPT_DIR/../.." &>/dev/null && pwd)"
REPO_ROOT="$(cd "$LIB_ROOT/../../.." &>/dev/null && pwd)"
BUILD_DIR="$REPO_ROOT/build"
ARCH_DIR="$LIB_ROOT/src/cortex-m7/fpv5-d16-hard"

echo "=== Generating Zenoh-Pico Arduino Library ==="
echo "Repo root: $REPO_ROOT"
echo "Build dir: $BUILD_DIR"
echo "Lib root:  $LIB_ROOT"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating CMake build directory..."
    mkdir -p "$BUILD_DIR"
    cmake -B "$BUILD_DIR" -S "$REPO_ROOT"
fi

echo "Building libs_zenoh_pico target..."
cmake --build "$BUILD_DIR" --target libs_zenoh_pico -j"$(nproc)"

echo "Copying precompiled static library archive..."
mkdir -p "$ARCH_DIR"
cp -v "$BUILD_DIR/libs/zenoh_pico/liblibs_zenoh_pico.a" "$ARCH_DIR/libzenoh_pico.a"

echo "Syncing Zenoh-Pico headers..."
cp -r "$REPO_ROOT/third_party/zenoh-pico/include/"* "$LIB_ROOT/src/"
cp -v "$BUILD_DIR/libs/zenoh_pico/include/zenoh-pico/config.h" "$LIB_ROOT/src/zenoh-pico/config.h"

# Ensure config.h has platform fallback defines for Arduino sketches
if ! grep -q "ZENOH_FREERTOS_LWIP" "$LIB_ROOT/src/zenoh-pico/config.h"; then
    sed -i '/#define INCLUDE_ZENOH_PICO_CONFIG_H/a \
#ifndef ZENOH_FREERTOS_LWIP\
#define ZENOH_FREERTOS_LWIP 1\
#endif\
#ifndef ZENOH_COMPILER_GCC\
#define ZENOH_COMPILER_GCC 1\
#endif\
#ifndef INET6_ADDRSTRLEN\
#define INET6_ADDRSTRLEN 46\
#endif\
#ifndef u32_t\
#include <stdint.h>\
typedef uint32_t u32_t;\
#endif' "$LIB_ROOT/src/zenoh-pico/config.h"
fi

echo "=== Zenoh-Pico Arduino Library successfully generated! ==="
