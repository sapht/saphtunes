#ifndef _MAIN__H_
#define _MAIN__H_
#define st _st

extern struct st_singleton {
    struct song_list *songs;
    struct album_list *albums;

    struct {
        char *song_dir;
        char *album_dir;

        char *song_git_dir;
        char *album_git_dir;
    } p;
} _st;


#endif
