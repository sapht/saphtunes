#include "cdk.h"
#ifndef _MAIN__H_
#define _MAIN__H_
#define cm _cm

extern struct cm_singleton {
    struct song_list *songs;
    struct album_list *albums;

    struct cdk cdk;
    struct {
        char *song_dir;
        char *album_dir;

        char *song_git_dir;
        char *album_git_dir;
    } p;
} _cm;


#endif
