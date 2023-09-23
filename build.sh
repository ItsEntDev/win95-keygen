#!/bin/bash
if ! command -v ninja >/dev/null 2>&1; then
    echo "Missing ninja."
    exit
fi
if ! command -v cmake >/dev/null 2>&1; then
    echo "Missing cmake."
    exit
fi
echo Building for "$1"
cmake -DCMAKE_BUILD_TYPE="$1" -S . -B . -G Ninja
$GEN
mv ./linux ./win95-keygen
echo Build finished.
