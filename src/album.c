#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "song.h"
#include "album.h"
#include "util.h"

struct album *
album_create(char *root, char *name)
{
    struct album *r = malloc(sizeof(struct album));

    git_load_generic(&r->git, root, name);
    r->path = r->git.path;
    r->m3u_path = malloc(PATH_MAX_LEN * sizeof(char));
    sprintf(r->m3u_path, "%s/%s/playlist.m3u", root, name);
    r->slug = name;

    /*printf("Album path is %s\n", r->path);*/

    return r;
}

void
album_make_m3u(struct album *album)
{
    char *buf = malloc(sizeof(char) * album->songs.len * PATH_MAX_LEN);
    FILE *fp = fopen(album->m3u_path, "w");
    struct song *song;
    for (int i=0; i<album->songs.len; i++) {
        song = album->songs.e[i];
        fwrite(song->render_path, strlen(song->render_path), 1, fp);
        fwrite("\n", 1, 1, fp);
    }

    fclose(fp);
}

void
album_list_make_m3u(struct album_list *album_list)
{
    struct album *album;
    for(int i=0; i<album_list->len; i++) {
        album = album_list->e[i];
        album_make_m3u(album);
    }
}



struct song_list *
album_list_find_exclusions(
    struct album_list *album_list,
    struct song_list *song_list)
{
	struct song_list orphans;
	orphans.e = malloc(1024 * sizeof(void*)); /* let's say max 1024 songs... */
	orphans.len = 0;

    for(int i=0; i<song_list->len; i++) {
		int found = 0;
		for(int j=0; j<album_list->len; j++) {
			for(int k=0; k<album_list->e[j]->songs.len; k++) {
				if (album_list->e[j]->songs.e[k] == song_list->e[i]) {
					found = 1;
				}
			}
		}

		if(!found) {
			orphans.e[
				orphans.len++
			] = song_list->e[i];
		}
	}

	struct song_list *retval = malloc(sizeof(orphans));
	*retval = orphans;
	return retval;
}

void
album_list_match_songs(struct album_list *album_list, 
                       struct song_list *song_list)
{
    /* XXX: Note that songs are pointers to instances in _st.songs.
     * This means the path is going to be the main clone, not the submodule
     * of the album. To implement things that require the submodule path, we
     * could create a album_song struct that represents the song embedded
     * within an album, and perhaps add another git struct to it.
     */

    struct album *album;
    struct song *song;


    char **submodules = malloc(GIT_SUBMODULE_MAX_NUM * sizeof(char*));
    int submodule_num;

    for (int ai=0; ai<album_list->len; ai++) {
        album = album_list->e[ai];

        submodule_num = git_repo_fill_submodules(&album->git, submodules);
        album->songs.e = malloc(submodule_num * sizeof(void*));


        int found;
        for (int j=0; j<submodule_num; j++) {
            found = 0;
            /*printf("Looking at submodule %s\n", submodules[j]);*/
            for (int i=0; i<song_list->len; i++) {
                song = song_list->e[i];
                if (0 == strcmp(submodules[j], song->git.origin)) {
                    album->songs.e[album->songs.len++] = song;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                fprintf(stderr, "FATAL ERROR: SUBMODULE NOT FOUND: %s\n", submodules[j]);
                exit(1);
            }
        }

        qsort(album->songs.e, 
              album->songs.len, 
              sizeof(struct song*),
              song_qsort);

        /*fprintf(stderr, 
                "Album %s has %d songs\n", 
                album->slug, 
                album->songs.len);*/
    }
}

int
album_load_dir(char* dir, struct album_list *album_list) {
    struct dirent_list files = dir_read_all(dir);

    album_list->e = malloc(ALBUM_MAX_NUM * sizeof(void*));
    album_list->len = files.len;

    for(int i=0; i<files.len; i++) {
        album_list->e[i] = album_create(
                dir, 
                files.e[i].d_name);
    }

    return album_list->len;
}
