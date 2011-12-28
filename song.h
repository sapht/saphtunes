#include "git.h"
#define SONG_DIR "/Volumes/Audio/Workspace/songs"
#define SONG_GIT_DIR "/Volumes/Audio/git/songs"
#define MAX_SONGS 4096

struct gitsong {
    char *path;
    char *slug;
    struct git_entry *repo;
};

struct song_list {
    int num;
    struct gitsong **entries;
};

struct song_list * song_list_from_dir(char *song_dir);
struct gitsong *song_create (char *slug, char *song_dir);

int song_repos_not_cloned(char **not_cloned, struct song_list *songs, char *git_dir);
