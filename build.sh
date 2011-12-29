#!/bin/sh
mkdir -p build
gcc -Wall -ggdb -std=c99 -I/usr/local/include/cdk -lncurses -lcdk *.c -o build/cm
