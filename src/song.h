#include <stdlib.h>
#include "git.h"

#ifndef _SONG__H_
#define _SONG__H_
#define SONG_MAX_NUM 2048

struct song {
    char* slug;
    char* path;
    struct git_repo git;
};

struct song_list {
    struct song **e;
    int len;
};

int song_qsort(struct song *a, struct song *b);
int songs_load_dir(char *dir, struct song_list *song_list);
struct song * song_create(char *root, char *name);
int song_repos_not_cloned(char *git_dir, struct song_list *song_list, char **not_cloned);
#endif
