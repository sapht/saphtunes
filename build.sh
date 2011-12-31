#!/bin/sh
mkdir -p build
gcc -pg -Wall -ggdb -std=c99 -I/usr/local/include/cdk -lncurses -lcdk *.c -o build/cm
