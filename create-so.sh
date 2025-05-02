#!/bin/bash
# -----------------------------------------------------------------------------
# @description
#   Build, run, and clean Chronicle project
# -----------------------------------------------------------------------------

set -euo pipefail
IFS=$'\n\t'

# Configuration
BUILD_DIR="build"
TARGET_BINARY="bin/chronicle.so"
TEST_DIR="tests"
PYTHON_SCRIPT="chronicle.py"
BIN_DIR="bin"

# Functions
function info {
    echo -e "\033[1;34m[INFO]\033[0m $*"
}

function error_exit {
    echo -e "\033[1;31m[ERROR]\033[0m $*" >&2
    exit 1
}

# Build process
info "Creating build directory..."
mkdir -p "$BUILD_DIR"

info "Entering build directory..."
cd "$BUILD_DIR"

info "Running cmake..."
cmake .. || error_exit "CMake failed."

info "Compiling project..."
make || error_exit "Make failed."

info "Leaving build directory..."
cd ..

# Clean up build artifacts
info "Removing build dir..."
rm -rf "$BUILD_DIR"
