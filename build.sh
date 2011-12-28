#!/bin/sh
mkdir -p build
gcc -Wall -ggdb -std=c99 *.c -o build/congman
