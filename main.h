#include "cdk.h"
#ifndef _MAIN__H_
#define _MAIN__H_

extern struct cm_singleton {
    CDKSCREEN *cdk_screen;
    WINDOW *curses_window;

    struct {
        CDKOBJS* left;
        CDKOBJS* right;
    } cdk_widgets;

    struct {
        char *song_dir;
        char *album_dir;

        char *song_git_dir;
        char *album_git_dir;
    } p;

    struct song_list *songs;
    struct album_list *albums;
} _cm;


#endif
