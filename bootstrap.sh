#!/bin/bash

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
ROOT_DIR="$(dirname "$SRC_DIR")"

# vcpkg
VCPKG_DIR="$ROOT_DIR/vcpkg"
rm -fr $VCPKG_DIR

git clone https://github.com/Microsoft/vcpkg.git $VCPKG_DIR

$VCPKG_DIR/bootstrap-vcpkg.sh

VCPKG_EXE="$VCPKG_DIR/vcpkg"
$VCPKG_EXE install sdl2 libjpeg-turbo libpng sdl2 glfw3 glad tinyxml2 --triplet x64-osx

VCPKG_TOOLCHAIN_PATH="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake"

# Generate the project.
BUILD_DIR="$ROOT_DIR/build"
rm -fr $BUILD_DIR
mkdir $BUILD_DIR

cmake -S $SRC_DIR -B $BUILD_DIR -G  "Xcode" "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_TOOLCHAIN_PATH"


