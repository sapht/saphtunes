#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "ui.h"
#include "song.h"
#include "album.h"

struct st_singleton _st;

static int load_data() {
    if ((st.p.song_dir      = getenv("SAPHTUNE_SONG_DIR")) == NULL ||
        (st.p.album_dir     = getenv("SAPHTUNE_ALBUM_DIR")) == NULL ||
        (st.p.song_git_dir  = getenv("SAPHTUNE_GIT_SONG_DIR")) == NULL ||
        (st.p.album_git_dir = getenv("SAPHTUNE_GIT_ALBUM_DIR")) == NULL) {
        fprintf(stderr, "Could not load paths from env\n");
        exit(1);
    }

    st.songs  = malloc(sizeof(struct song_list));
    st.albums = malloc(sizeof(struct song_list));

    printf("Loading songs...\n");
    if(0==songs_load_dir(_st.p.song_dir, _st.songs)) return 0;
    printf("Loading albums...\n");
    if(0==album_load_dir(_st.p.album_dir, _st.albums)) return 0;

    printf("Matching album repos to songs...\n");
    album_list_match_songs(_st.albums, _st.songs);

    return 1;
}


static int main_from_args(int argc, char **argv)
{
    if (strcmp(argv[1], "orphans") == 0) {
        if(!load_data()) {
            fprintf(stderr, "Could not create song/album lists\n");
            exit(1);
        }

        struct song_list*orphans = album_list_find_exclusions(
            st.albums, st.songs
        );
        if (orphans->len > 0) {
            for (int i=0; i<orphans->len; i++) {
                printf("%s\n", orphans->e[i]->slug);
            }
        }
    } else if (strcmp(argv[1], "analyze") == 0) {
        printf("Supercool analyze finder initialized\n");
        struct song_render_stat stat = song_render_analyze(argv[2]);

        printf("Nullspace: %d, clipping: %d",
            stat.nullspace,
            stat.clipping);

    } else if (strcmp(argv[1], "displaced") == 0) {
        if(!load_data()) {
            fprintf(stderr, "Could not create song/album lists\n");
            exit(1);
        }

        int num_not_cloned = 0;
        char** not_cloned = malloc(sizeof(void*) * SONG_MAX_NUM);
        num_not_cloned = song_repos_not_cloned(
                _st.p.song_git_dir,
                _st.songs,
                not_cloned
                );
        if (num_not_cloned > 0) {
            for (int i=0; i<num_not_cloned; i++) {
                printf("%s\n", not_cloned[i]);
            }
        }
    } else {
        printf("Commands:\n");
        printf("displaced\n");
        printf("orphans\n");
    }    

    return 0;
}

int main(int argc, char** argv) {

	if (argc == 1) {
		/* This means interactive mode */
        if(!load_data()) {
            fprintf(stderr, "Could not create song/album lists\n");
            exit(1);
        }

        return ui_main(&argc, &argv);
	} else {
        return main_from_args(argc, argv);
	}

    printf("Nope\n");
    return 0;
}
