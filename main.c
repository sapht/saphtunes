#include <stdio.h>

#include "main.h"
#include "cdk.h"
#include "song.h"
#include "album.h"

struct cm_singleton _cm;

static int load_data() {
    _cm.songs = malloc(sizeof(struct song_list));
    _cm.albums = malloc(sizeof(struct song_list));

    if(0==songs_load_dir(_cm.p.song_dir, _cm.songs)) return 0;
    if(0==album_load_dir(_cm.p.album_dir, _cm.albums)) return 0;

    album_list_match_songs(_cm.albums, _cm.songs);

    return 1;
}


static int main_from_arg(char* arg)
{
    if (strcmp(arg, "orphans") == 0) {
        struct song_list *orphans = album_list_find_exclusions(
            _cm.albums, _cm.songs
        );
        if (orphans->len > 0) {
            for (int i=0; i<orphans->len; i++) {
                printf("%s\n", orphans->e[i]->slug);
            }
        }
    } else if (strcmp(arg, "displaced") == 0) {
        int num_not_cloned = 0;
        char** not_cloned = malloc(sizeof(void*) * SONG_MAX_NUM);
        num_not_cloned = song_repos_not_cloned(
                _cm.p.song_git_dir,
                _cm.songs,
                not_cloned
                );
        if (num_not_cloned > 0) {
            for (int i=0; i<num_not_cloned; i++) {
                printf("%s\n", not_cloned[i]);
            }
        }
    } else if (strcmp(arg, "help") == 0) {
        printf("Commands:\n");
        printf("displaced\n");
        printf("orphans\n");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    _cm.p.song_dir = "/tmp/cm_test/songs";
    _cm.p.album_dir = "/tmp/cm_test/album";
    _cm.p.song_git_dir = "/tmp/cm_test/origin/songs";
    _cm.p.album_git_dir = "/tmp/cm_test/origin/album";
    if(0 == load_data()) {
        fprintf(stderr, "Could not create song/album lists\n");
        exit(1);
    }

	if (argc == 1) {
		/* This means interactive mode */
        return ui_main();
	} else {
        return main_from_arg(argv[1]);
	}
}
