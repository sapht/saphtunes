#!/bin/sh
CFLAGS="-I../ -I/usr/local/include/cdk -lncurses -lcdk"
gcc $CFLAGS test_cdk.c -o test_cdk.o
CFLAGS="-I../"
gcc $CFLAGS ../git.c test_git.c -o test_git.o
