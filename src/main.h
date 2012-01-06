#ifndef _MAIN__H_
#define _MAIN__H_
#define st _st
#include "cache.h"

extern struct st_singleton {
    struct song_list *songs;
    struct album_list *albums;
    struct cache_entry cache[8192];

    int cache_num;
    struct hash *cachetable[2048];

    struct {
        char *song_dir;
        char *album_dir;

        char *song_git_dir;
        char *album_git_dir;

        char *cache;
    } p;
} _st;


#endif
