#!/bin/bash
if [ "$2" == "Ninja" ]; then
	GEN="ninja"
elif [ "$2" == "Make" ]; then
	GEN="make"
fi
echo Building for "$1"
cmake -DCMAKE_BUILD_TYPE="$1" -S . -B . -G "$2"
$GEN
mv ./linux ./win95-keygen
echo Build finished.