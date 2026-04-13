#!/usr/bin/env bash

ulimit -c unlimited

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

cmake -DCMAKE_BUILD_TYPE=Release -S "$SCRIPT_DIR" -B "$BUILD_DIR"

cmake --build "$BUILD_DIR" -- -j"$(nproc)"

cd "$SCRIPT_DIR/resources"

"$BUILD_DIR/AtmosphereLutGenerator"
