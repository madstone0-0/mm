#!/usr/bin/env bash

RELEASE_DIR="build_rel"
DEBUG_DIR="build_debug"

build_release() {
	echo "Building Release"
	mkdir -p "$RELEASE_DIR"
	CXX=g++ cmake -G 'Ninja' -S . -B "$RELEASE_DIR" -DCMAKE_BUILD_TYPE=Release && ninja -C "$RELEASE_DIR"/ &>release_build.log
}

build_debug() {
	echo "Building Debug"
	mkdir -p "$DEBUG_DIR"
	CXX=g++ cmake -G 'Ninja' -S . -B "$DEBUG_DIR" -DCMAKE_BUILD_TYPE=Debug && ninja -C "$DEBUG_DIR"/ &>build.log
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
esac
