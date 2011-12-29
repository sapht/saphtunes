#include <stdio.h>
#include "song.h"
#include "album.h"
#include "util.h"

struct album *
album_create(char *root, char *name)
{
    struct album *r = malloc(sizeof(struct album));
    r->slug = name;
    git_load_generic((struct git_repo*)r, root, name);
    r->path = r->git.path;
    return r;
}

struct song_list *
album_list_find_exclusions(
    struct album_list *album_list,
    struct song_list *song_list)
{
    return song_list;
}

void
album_list_match_songs(struct album_list *album_list, 
                 struct song_list *song_list)
{
    /* magic... */
}

int
album_load_dir(char* dir, struct album_list *album_list) {
    struct dirent_list files = dir_read_all(dir);

    album_list->e = malloc(ALBUM_MAX_NUM * sizeof(void*));
    album_list->len = files.len;

    for(int i=0; i<files.len; i++) {
        album_list->e[i] = album_create(
                dir, 
                files.e[i].d_name);
    }

    return album_list->len;
}
