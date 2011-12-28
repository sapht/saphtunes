#define ALBUM_DIR "test/album"
#define MAX_ALBUMS 512
#define MAX_ALBUM_SONGS 128

struct gitalbum {
    char *path;
    char *slug;
    struct song_list *songs;
};

struct album_list {
    int num;
    struct gitalbum **entries;
};
