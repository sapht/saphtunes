#include <stdio.h>

#include "cdk.h"
#include "song.h"
#include "album.h"

extern struct cm_singleton {
    CDKSCREEN *cdk_screen;
    WINDOW *curses_window;

    struct cdk_widgets {
        CDKOBJS* left;
        CDKOBJS* right;
    } w;

    struct song_list *songs;
    struct album_list *album;
} _cm;


int load_data() {
    _cm.songs = songs_load_dir("test/songs");
    if (0 == _cm.songs->len) return 0;

    _cm.album = album_load_dir("test/album");
    if (0 == _cm.album->len) return 0;

    return 1;
}


int main_from_arg(char* arg)
{
    if (strcmp(argv[1], "orphans") == 0) {
        struct song_list *orphans = album_find_exclusions(albums, songs);
        if (orphans->num > 0) {
            for (int i=0; i<orphans->num; i++) {
                printf("%s\n", orphans->entries[i]->slug);
            }
        }
    } else if (strcmp(argv[1], "displaced") == 0) {
        int num_not_cloned = 0;
        char** not_cloned = malloc(sizeof(void*) * MAX_SONGS);
        num_not_cloned = song_repos_not_cloned(not_cloned, songs, SONG_GIT_DIR);
        if (num_not_cloned > 0) {
            for (int i=0; i<num_not_cloned; i++) {
                printf("%s\n", not_cloned[i]);
            }
        }
    } else if (strcmp(argv[1], "help") == 0) {
        printf("Commands:\n");
        printf("displaced\n");
        printf("orphans\n");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    if(0 == load_data()) {
        fprintf(stderr, "Could not create song/album lists\n");
        exit(1);
    }

	if (argc == 1) {
		/* This means interactive mode */
        return ui_main();
	} else {
        return run_from_arg(argv[1]);
	}
}
