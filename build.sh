#!/bin/sh
mkdir -p build
gcc src/*.c -o build/cm -std=c99 -pg -Wall -ggdb -lncurses -lcdk `pkg-config --libs --cflags gtk+-2.0`
