#include <stdio.h>
#include "song.h"
#include "album.h"
#include "util.h"

struct album *
album_create(char *root, char *name)
{
    struct album *r = malloc(sizeof(struct album));

    printf("ALBUM\n-----------------------------------------\n");
    git_load_generic((struct git_repo*)r, root, name);
    r->path = r->git.path;
    r->slug = name;

    printf("Album Path %s has origin %s\n", r->git.path, r->git.origin);

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
    /* XXX: Note that songs are pointers to instances in _cm.songs.
     * This means the path is going to be the main clone, not the submodule
     * of the album. To implement things that require the submodule path, we
     * could create a album_song struct that represents the song embedded
     * within an album, and perhaps add another git struct to it.
     */

    struct album *album;
    struct song *song;


    for (int ai=0; ai<album_list->len; ai++) {
        album = album_list->e[ai];
        album->songs.e = malloc(SONG_MAX_NUM * sizeof(void*));

        for (int i=0; i<song_list->len; i++) {
            song = song_list->e[i];

            /*printf("Looking for submodule with origin %s\n", song->git.origin);*/
            if (git_repo_has_submodule((struct git_repo*) album,
                                       song->git.origin)) {
                album->songs.e[
                    album->songs.len++
                ] = song;
            }
        }

        fprintf(stderr, 
                "Album %s has %d songs\n", 
                album->slug, 
                album->songs.len);
    }
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
