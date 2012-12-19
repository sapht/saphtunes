#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "cache.h"
#include "ui.h"

struct st_singleton _st;

static int load_data(int use_cache) {
    if ((st.p.song_dir      = getenv("SAPHTUNE_SONG_DIR")) == NULL ||
        (st.p.playlist_dir  = getenv("SAPHTUNE_PLAYLIST_DIR")) == NULL ||
        (st.p.album_dir     = getenv("SAPHTUNE_ALBUM_DIR")) == NULL ||
        (st.p.song_git_dir  = getenv("SAPHTUNE_GIT_SONG_DIR")) == NULL ||
        (st.p.cache         = getenv("SAPHTUNE_CACHE_FILE")) == NULL ||
        (st.p.album_git_dir = getenv("SAPHTUNE_GIT_ALBUM_DIR")) == NULL) {
        fprintf(stderr, "Could not load paths from env\n");
        return 0;
    }

    if(use_cache) {
        cache_load(st.p.cache);
    }

    st.songs  = malloc(sizeof(struct song_list));
    st.albums = malloc(sizeof(struct song_list));

    if(0==songs_load_dir(_st.p.song_dir, _st.songs)) return 0;
    if(0==album_load_dir(_st.p.album_dir, _st.albums)) return 0;
    if(0==album_list_match_songs(_st.albums, _st.songs)) return 0;

    return 1;
}

void need_data() {
    if(!load_data(1)) {
        fprintf(stderr, "Could not create song/album lists\n");
        exit(1);
    }
}

void data_available() {
	cache_dump(st.p.cache);
}

static int render_analyze(char *render_path) {
	struct song_render_stat s = song_render_analyze(render_path);
	song_render_print(&s);
	return 0;
}

static int print_nullspaced() {
	need_data();
	for(int i=0; i<st.songs->len; i++) {
		if(st.songs->e[i]->render_stat.nullspace) {
			printf("%s\n", st.songs->e[i]->slug);
		}
	}
	data_available();
	return 0;
}

int main(int argc, char** argv) {
    int opts;
	int retval = -1;
	struct {
		char list[10];
		char* desc[10];
	} o = {
		"oapsdlgnr", { 
			"         Print orphans",
			" [file]  Analyze render",
			" [file]  Print song meta",
			"         Print git status",
			"         Print displaced songs",
			"         Write playlist files",
			"         Open GUI",
			"         List nullspaced songs",
			"         Recache",
			0
		}
	};
	void *tmp;

    while( -1 != (opts = getopt(argc, argv, o.list))) {
        switch(opts) {
		case 'n':
			retval = print_nullspaced();
			break;

        case 'o':
            need_data();
			data_available();

            song_list_print(album_list_find_exclusions(st.albums, st.songs), 0);
            retval = 0;
			break;

        case 'a':
            retval = render_analyze(argv[2]);
			break;

        case 'p':
            need_data();
            data_available();
            retval = song_print_metadata(song_resolve_slug(argv[2]));
			break;

        case 's':
            need_data();
			data_available();

			printf("slug\tstat\n");
            for(int i=0; i<st.songs->len; i++) {
                if(0 != strcmp("", st.songs->e[i]->git.status)) {
                    printf("%s\n", st.songs->e[i]->slug); 
                    putchar('\t');
                    for (int j=0; j<strlen(st.songs->e[i]->git.status) - 1; j++) {
                        if (st.songs->e[i]->git.status[j] == '\n') {
                            putchar('\n');
                            putchar('\t');
                        } else {
                            putchar(st.songs->e[i]->git.status[j]);
                        }
                    }
                    putchar('\n');
                }
            }

            retval = 0;
			break;

        case 'd': 
            need_data();
            data_available();

            int num_not_cloned = 0;
            char** not_cloned = malloc(sizeof(void*) * SONG_MAX_NUM);
            num_not_cloned = song_repos_not_cloned(
				st.p.song_git_dir,
				st.songs,
				not_cloned
			);

            if (num_not_cloned > 0) {
                for (int i=0; i<num_not_cloned; i++) {
                    printf("%s\n", not_cloned[i]);
                }
            }

            retval = 0;
			break;

        case 'l':
            need_data();

            printf("album.m3u\n");
			tmp = malloc(PATH_MAX);
            sprintf( (char*) tmp, "%s/album.m3u", st.p.playlist_dir);
			album_list_make_m3u(_st.albums, (char*) tmp);

            printf("songs.m3u\n");
            sprintf( (char*) tmp, "%s/songs.m3u", st.p.playlist_dir);
			song_list_make_m3u(_st.songs, (char*) tmp);

			free(tmp);
            data_available();
            retval = 0;
			break;

        case 'g':
            need_data();
            data_available();
            retval = ui_main(&argc, &argv);
			break;

        case 'r':
            load_data(0);
            data_available();
            printf("%s\n", st.p.cache);
            retval = 0;
			break;
		}
    }

	if(retval != -1)  {
		return retval;
	}

	fprintf(stderr, "Usage:\n");
	for(int i=0; o.list[i] != 0; i++) {
		fprintf(stderr, "\t-%c%s\n", o.list[i], o.desc[i]);
	}
	return 1;
}
