#!/usr/bin/env bash

RELEASE_DIR="build_rel"
DEBUG_DIR="build_debug"
ARDUINO="build_ardinuo"
COMPILER_DEB="clang++"
COMPILER_REL="g++"
COMPIER_ARD="/home/mads/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++"

build_release() {
	echo "Building Release"
	mkdir -p "$RELEASE_DIR"
	CXX=$COMPILER_REL cmake -G 'Ninja' -S . -B "$RELEASE_DIR" -DCMAKE_BUILD_TYPE=Release && ninja -C "$RELEASE_DIR"/ &>release_build.log
}

build_debug() {
	echo "Building Debug"
	mkdir -p "$DEBUG_DIR"
	CXX=$COMPILER_DEB cmake -G 'Ninja' -S . -B "$DEBUG_DIR" -DCMAKE_BUILD_TYPE=Debug && ninja -C "$DEBUG_DIR"/ &>build.log
}

build_ardinuo() {
	echo "Building Arduino"
	mkdir -p "$ARDUINO"
	CXX=$COMPIER_ARD cmake -G 'Ninja' -S . -B "$ARDUINO" -DCMAKE_BUILD_TYPE=Debug -DARDUINO=ON && ninja -C "$ARDUINO"/ &>ard.log
}

build_ardinuo_cmake() {
	echo "Building Arduino cmake"
	mkdir -p "$ARDUINO"
	CXX=$COMPIER_ARD cmake -G 'Ninja' -S . -B "$ARDUINO" -DCMAKE_BUILD_TYPE=Release -DARDUINO_CMAKE=ON && ninja -C "$ARDUINO"/ &>ard.log
}

MODE="$1"

if [ -z "$MODE" ]; then
	MODE="debug"
fi

case $MODE in
"debug")
	build_debug
	;;
"release")
	build_release
	;;
"arduino")
	build_ardinuo
	;;
"arduino_cmake")
	build_ardinuo_cmake
	;;
esac
