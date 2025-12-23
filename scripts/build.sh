#!/bin/bash
set -e

if [ ! -d "build" ]; then
    ./scripts/configure.sh
fi

cd build
make -j$(nproc)
echo "Build complete."
