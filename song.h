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

