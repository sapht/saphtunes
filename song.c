#include "song.h"
#include "util.h"

struct song *
song_create(char *root, char *name)
{
    struct song *r = malloc(sizeof(struct song));
    r->slug = name;
    git_load_generic((struct git_repo*)r, root, name);
    return r;
}

int
songs_load_dir(char* dir, struct song_list *song_list) {
    struct dirent_list *files = dir_read_all(dir);

    song_list = malloc(SONG_MAX_NUM * sizeof(void*));
    song_list->len = files->len;

    for(int i=0; i<files->len; i++) {
        song_list->e[i] = song_create(
                dir, 
                files->e[i]->d_name);
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
