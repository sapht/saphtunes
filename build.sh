#!/bin/sh
mkdir -p build
find src \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 gcc -o build/st -std=c99 -pg -Wall -ggdb -lncurses -lcdk `pkg-config --libs --cflags gtk+-2.0`
