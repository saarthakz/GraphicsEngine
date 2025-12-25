#!/bin/bash
set -e

# Detect OS
OS="$(uname -s)"

case "${OS}" in
    Darwin*)
        echo "Detected macOS"
        if ! command -v brew &> /dev/null; then
            echo "Error: Homebrew not found. Please install it from https://brew.sh/"
            exit 1
        fi
        echo "Installing dependencies via Homebrew..."
        brew install cmake glfw
        ;;
    Linux*)
        if [ -f /etc/debian_version ]; then
            echo "Detected Ubuntu/Debian"
            sudo apt-get update
            sudo apt-get install -y build-essential cmake libglfw3-dev libgl1-mesa-dev
        else
            echo "Detected Linux, but not a Debian-based system (apt-get not supported)."
            echo "Please install 'cmake', 'glfw', and 'opengl' development headers manually."
            exit 1
        fi
        ;;
    *)
        echo "Unsupported OS: ${OS}"
        exit 1
        ;;
esac

echo "Setup complete. You can now build the project using ./scripts/build.sh"
