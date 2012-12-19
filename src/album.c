#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "main.h"
#include <sys/types.h>

struct album *
album_create(char *root, char *name)
{
    struct album *r = malloc(sizeof(struct album));

    git_load_generic(&r->git, root, name);
    r->path = r->git.path;
    r->m3u_path = malloc(PATH_MAX_LEN * sizeof(char));
    sprintf(r->m3u_path, "%s/play-%s.m3u", root, name);
    r->slug = name;

    r->length = 0;
    r->songs.len = 0;
    /*printf("Album path is %s\n", r->path);*/

    return r;
}

int
album_make_m3u(struct album *album)
{
    return song_list_make_m3u(
        &album->songs,
        album->m3u_path
    );
}

int
album_list_make_m3u(struct album_list *album_list, char *path)
{
    FILE *album_master;
    album_master = fopen(path, "w");
    fwrite("#EXTM3U\n", strlen("#EXTM3U\n"), 1, album_master);
    struct album *album;
    for(int i=0; i<album_list->len; i++) {
        album = album_list->e[i];
        album_make_m3u(album);
        if(album->songs.len > 0) {
            fprintf(album_master, "#EXTINF:%d, sapht - %s\n",
                album->length,
                album->slug);
            fprintf(album_master, "%s\n", album->m3u_path);
        }
    }

    fclose(album_master);
    return 1;
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
            if (album_is_auxiliary(album_list->e[j])) {
                continue;
            }

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

int
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
    char *entry_link_target;
    char *entry_full_path;
    struct dirent *dp;
    struct dirent_list album_dir_entries;


    for (int ai=0; ai<album_list->len; ai++) {
        album = album_list->e[ai];

        album_dir_entries = dir_read_all(album->path, dir_filter_is_symlink);
        dirent_list_sort(&album_dir_entries);

        album->songs.e = malloc(album_dir_entries.len * sizeof(void*));
        /* allocate space for all items, realloc when we set len */

        for (int j=0; j<album_dir_entries.len; j++) {
            dp = &album_dir_entries.e[j];

            entry_full_path = malloc(sizeof(char) * (2 + strlen(album->path) + dp->d_namlen));
            sprintf(entry_full_path, "%s/%s", 
                album->path,
                dp->d_name
            );

            entry_link_target = realpath(entry_full_path, 0);
            free(entry_full_path);

            if(0 == entry_link_target) { /* Could not resolve path name */
                fprintf(stderr, "ERROR: SONGLINK UNRESOLVED: %s\n", dp->d_name);
                return 0;
            }

            if( 0 == strncmp(entry_link_target, st.p.song_dir, strlen(st.p.song_dir))) {
                /* might be a song link */
                for (int i=0; i<song_list->len; i++) {
                    if (0 == strcmp(entry_link_target, song_list->e[i]->path)) {
                        album->songs.e[album->songs.len] = song_list->e[i];
                        /*printf("%s at %d\n", song_list->e[i]->slug, album->songs.len);*/
                        album->songs.len++;
                        album->length += song_list->e[i]->render_stat.duration;
                        break;
                    }
                }
            }
        }

        album->songs.e = realloc(album->songs.e, sizeof(void*) * album->songs.len);
    }

    return 1;
}

int
album_load_dir(char* dir, struct album_list *album_list) {
    struct dirent_list files = dir_read_all(dir, dir_filter_is_dir);

    album_list->e = malloc(ALBUM_MAX_NUM * sizeof(void*));
    album_list->len = files.len;

    for(int i=0; i<files.len; i++) {
        album_list->e[i] = album_create(
                dir, 
                files.e[i].d_name);
    }

    return album_list->len;
}

int
album_is_auxiliary(struct album *album) {
    if(strstr(album->slug, "github")) {
        return 1;
    }
    if(strstr(album->slug, ".edits")) {
        return 1;
    }
    return 0;
}
