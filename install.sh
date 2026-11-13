#!/usr/bin/env bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if command -v makepkg >/dev/null 2>&1; then
    makepkg -si --noconfirm
elif command -v cmake >/dev/null 2>&1; then
    cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build build -j$(nproc 2>/dev/null || echo 2)
    sudo cmake --install build
else
    make -j$(nproc 2>/dev/null || echo 2)
    sudo make install
fi
