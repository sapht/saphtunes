#!/bin/sh
mkdir -p build
if [ "x$1" == "x" ]; then
    find src \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 gcc -o build/st -std=c99 -pg -Wall -ggdb -lncurses -lcdk `pkg-config --libs --cflags gtk+-2.0`
else
    cat guiscripts/_common.applescript \
        guiscripts/_reasonbatch.applescript \
        guiscripts/resave.applescript | osacompile -o build/resave.scpt

    cat guiscripts/_common.applescript \
        guiscripts/_reasonbatch.applescript \
        guiscripts/render.applescript | osacompile -o build/render.scpt
fi;
