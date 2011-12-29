#include <stdlib.h>
#include "git.h"

#ifndef _ALBUM__H_
#define _ALBUM__H_
#define ALBUM_MAX_NUM 256

struct album {
    struct git_repo git;
    struct song_list *songs;
    char *slug;
    char *path;
};

struct album_list {
    struct album **e;
    int len;
};

int album_load_dir(char* dir, struct album_list *album_list);
void album_list_match_songs(struct album_list *album_list, struct song_list *song_list);
struct album * album_create(char *root, char *name);
struct song_list * album_list_find_exclusions( struct album_list *album_list, struct song_list *song_list);
#endif
