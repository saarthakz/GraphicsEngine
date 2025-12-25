#!/bin/bash
set -e

if [ ! -d "build" ]; then
    ./scripts/configure.sh
fi

cd build
if [[ "$OSTYPE" == "darwin"* ]]; then
    THREADS=$(sysctl -n hw.logicalcpu)
else
    THREADS=$(nproc)
fi

make -j$THREADS
echo "Build complete."
