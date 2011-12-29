#!/bin/sh
CFLAGS="-std=c99 -ggdb -Wall -I../ -I/usr/local/include/cdk -lncurses -lcdk"
gcc $CFLAGS test_cdk.c -o test_cdk.o
CFLAGS="-std=c99 -ggdb -Wall -I../ -I/usr/local/include/cdk -lncurses -lcdk"
gcc $CFLAGS test_cdk_split.c -o test_cdk_split.o
CFLAGS="-std=c99 -ggdb -Wall -I../"
gcc $CFLAGS ../git.c test_git.c -o test_git.o
