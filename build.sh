#!/bin/bash
if [ "$1" == "Release" ]; then
	BINARY_DIR="./cmake-build-release"
elif [ "$1" == "Debug" ]; then
	BINARY_DIR="./cmake-build-debug"
else
	BINARY_DIR="."
fi

echo Building for $1
cmake -DCMAKE_BUILD_TYPE=$1 -S . -B . -G $2
mv "$BINARY_DIR/win95-keygen" "./win95-keygen"
