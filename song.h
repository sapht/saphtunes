#ifndef _SONG__H_
#define _SONG__H_
#include "git.h"

struct song {
    struct git_repo git;
    char* slug;
}

struct song_list {
    struct song **e;
    int l;
}

int
songs_load_dir(char* dir, struct **song song_list) {
#endif
