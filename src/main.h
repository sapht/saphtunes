#ifndef _MAIN__H_
#define _MAIN__H_

#include <stdio.h>
#define st _st
#define SONG_MAX_NUM 2048
#define ALBUM_MAX_NUM 256
#define GIT_SUBMODULE_MAX_NUM 256

#include "cache.h"

extern struct st_singleton {
    struct song_list *songs;
    struct album_list *albums;
    struct cache_entry cache[8192];

    int cache_num;
    struct hash *cachetable[2048];

    struct {
        char *song_dir, *album_dir, *playlist_dir, *song_git_dir, *album_git_dir, *cache;
    } p;
} _st;

struct git_repo {
	char *origin, *config_path, *path, *status;
};

struct song_render_stat {
    int nullspace, clipping, duration;
};

struct song {
    char *slug, *path, *render_path, *project_path;
    int render_path_len, project_path_len;
    struct git_repo git;
    struct song_render_stat render_stat;
};

struct song_list {
    struct song **e;
    int len;
};

struct album {
    char *slug, *path, *m3u_path;
    int length;
    int *song_order;
    struct git_repo git;
    struct song_list songs;
};

struct album_list {
    struct album **e;
    int len;
};


int song_list_make_m3u(struct song_list* song_list, char *m3u_path);
int song_slug_parse_order(char *song_slug);
struct song_render_stat song_render_analyze(char *render_path);
int song_qsort(struct song **a, struct song **b);
int songs_load_dir(char *dir, struct song_list *song_list);
struct song * song_create(char *root, char *name);
struct song * song_find_by_pathname(char *, struct song_list*);
int song_repos_not_cloned(char *git_dir, struct song_list *song_list, char **not_cloned);
void song_list_render(struct song_list *, char *);
void song_list_print(struct song_list *, FILE *);
void song_render_print(struct song_render_stat *);

int album_make_m3u(struct album *album);
int album_list_make_m3u(struct album_list *album_list, char *path);
int album_load_dir(char* dir, struct album_list *album_list);
int album_list_match_songs(struct album_list *album_list, struct song_list *song_list);
int album_is_auxiliary(struct album *album);
struct album * album_create(char *root, char *name);
struct song_list * album_list_find_exclusions( struct album_list *album_list, struct song_list *song_list);

int git_load_status(struct git_repo *repo);
int git_load_config(struct git_repo *repo);
int git_load_generic(struct git_repo *repo, char *root, char *path);
int git_repo_has_submodule(struct git_repo *repo, char *expected_origin);
int git_repo_fill_submodules(struct git_repo *root_repo, char **paths);

#endif
