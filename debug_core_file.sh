#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

core_file=$(ls -t /tmp/core* 2>/dev/null | head -n 1 || true)

if [[ -z "$core_file" ]]; then
    echo "No core file found."
    exit 0
fi

gdb "$SCRIPT_DIR/build/AtmosphereLutGenerator" "$core_file"
