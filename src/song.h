#include <stdlib.h>
#include "git.h"

#ifndef _SONG__H_
#define _SONG__H_
#define SONG_MAX_NUM 2048

struct song_render_stat {
    int nullspace;
    int clipping;
    int duration;
};

struct song {
    char *slug;
    char *path;
    struct git_repo git;

    struct song_render_stat render_stat;
    char *render_path;
    int render_path_len;

    char *project_path;
    int project_path_len;
};

struct song_list {
    struct song **e;
    int len;
};

struct song_render_stat song_render_analyze(char *render_path);
int song_qsort(struct song **a, struct song **b);
int songs_load_dir(char *dir, struct song_list *song_list);
struct song * song_create(char *root, char *name);
int song_repos_not_cloned(char *git_dir, struct song_list *song_list, char **not_cloned);
void song_list_render(struct song_list *song_list, char *render_script_path);
#endif
