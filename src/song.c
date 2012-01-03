#include <stdlib.h>
#include <stdio.h>
#include "song.h"
#include "util.h"
#include <string.h>

int
song_qsort(struct song *a,
                struct song *b)
{
    return strcmp(a->slug, b->slug);
}

struct song *
song_create(char *root, char *name)
{
    struct song *r = malloc(sizeof(struct song));

    int could_load = git_load_generic(&r->git, root, name);
    if(!could_load) {
        printf("Could not load %s\n", name);
        exit(1);
    }

    git_load_status(&r->git);
    r->path = r->git.path;
    r->slug = name;

    return r;
}

int
songs_load_dir(char* dir, struct song_list *song_list) {
    struct dirent_list files = dir_read_all(dir);

    song_list->e = malloc(SONG_MAX_NUM * sizeof(void*));
    song_list->len = files.len;

    /*fprintf(stderr, "Length: %d (dir: %s)\n", files.len, dir);*/

    for(int i=0; i<files.len; i++) {
        song_list->e[i] = song_create(
                dir, 
                files.e[i].d_name);
    }

    return song_list->len;
}

int
song_repos_not_cloned(char *git_dir,
                      struct song_list *song_list,
                      char **not_cloned)
{
    return 0;
}
