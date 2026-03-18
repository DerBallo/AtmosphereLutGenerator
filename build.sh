#!/usr/bin/env bash

set -e

ulimit -c unlimited

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
EXECUTABLE="$BUILD_DIR/AtmosphereLutGenerator"

MODE="$1"

if [[ -z "$MODE" ]]; then
    echo "Usage: $0 <debug | release | core>"
    exit 1
fi

clean_core_files() {
    rm -f /tmp/core.*
}

build_debug() {
    clean_core_files

    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-g3 -O0 -fno-omit-frame-pointer -Wall -Wextra -Wpedantic" -DCMAKE_EXE_LINKER_FLAGS_DEBUG="-rdynamic" -S "$SCRIPT_DIR" -B "$BUILD_DIR"

    cmake --build "$BUILD_DIR" -- -j"$(nproc)"

    #cmake --install "$BUILD_DIR" --prefix "$SCRIPT_DIR"

    echo "Running application..."

    cd "$SCRIPT_DIR/resources"

    "$EXECUTABLE"
}

build_release() {
    clean_core_files

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -mtune=native -DNDEBUG -flto -fno-omit-frame-pointer" -DCMAKE_EXE_LINKER_FLAGS_RELEASE="-s -flto" -S "$SCRIPT_DIR" -B "$BUILD_DIR"

    cmake --build "$BUILD_DIR" --verbose -- -j"$(nproc)"

    #cmake --install "$BUILD_DIR" --prefix "$SCRIPT_DIR"

    echo "Running application..."

    cd "$SCRIPT_DIR/resources"

    "$EXECUTABLE"
}

debug_core() {
    echo "Searching for latest core dump in /tmp..."
    core_file=$(ls -t /tmp/core* 2>/dev/null | head -n 1 || true)

    if [[ -z "$core_file" ]]; then
        echo "No core file found."
        exit 0
    fi

    echo "Found core file: $core_file"
    echo "Launching GDB..."
    gdb "$EXECUTABLE" "$core_file"
}

case "$MODE" in
    debug)
        build_debug
        ;;
    release)
        build_release
        ;;
    core)
        debug_core
        ;;
    *)
        echo "Invalid mode: $MODE"
        echo "Usage: $0 <debug | release | core>"
        ;;
esac
