#!/bin/bash
set -e

./scripts/build.sh

echo "Running GraphicsEngine..."
./build/GraphicsEngine
