#include <stdlib.h>
#include "git.h"

#ifndef _SONG__H_
#define _SONG__H_
#define SONG_MAX_NUM 1024

struct song {
    struct git_repo git;
    char* slug;
    char* path;
};

struct song_list {
    struct song **e;
    int len;
};

int songs_load_dir(char *dir, struct song_list *song_list);
struct song * song_create(char *root, char *name);
int song_repos_not_cloned(char *git_dir, struct song_list *song_list, char **not_cloned);
#endif
