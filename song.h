#define SONG_DIR "test/songs"
#define MAX_SONGS 4096

struct gitsong {
    char *path;
    char *slug;
    char *remote;
};

struct song_list {
    int num;
    struct gitsong **entries;
};

struct song_list * song_list_from_dir(char *song_dir);
struct gitsong *song_create (char *slug, char *song_dir);
