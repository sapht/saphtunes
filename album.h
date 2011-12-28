#include "song.h"
#include "git.h"
#define ALBUM_DIR "/Volumes/Audio/Workspace/album"
#define MAX_ALBUMS 512
#define MAX_ALBUM_SONGS 128

struct gitalbum {
    char *path;
    char *slug;
	struct git_entry *repo;
    struct song_list *songs;
};

struct album_list {
    int num;
    struct gitalbum **entries;
};


struct gitalbum *album_create(char*, char*);
struct album_list *album_list_from_dir(char*);

void album_load_songs(struct gitalbum*, struct song_list*);
int album_has_song(struct gitalbum*, struct gitsong*);

struct song_list * album_find_exclusions(struct album_list *albums, struct song_list *songs);
