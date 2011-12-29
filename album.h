#include "git.h"

#ifndef _ALBUM__H_
#define _ALBUM__H_

struct album {
    struct git_repo git;
    struct song_list *songs;
    char *slug;
}

struct album_list {
    struct album **e;
    int l;
}

#endif

