#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include "album.h"

struct gitalbum *
album_create(char* slug, char* album_dir) 
{
    struct gitalbum *album = malloc(sizeof(struct gitalbum));

    album->slug = strdup(slug);
    album->path = malloc(2 + strlen(album_dir) + strlen(slug)); /* slash and null at $ */
    sprintf(album->path, "%s/%s", album_dir, slug);

    album->repo = git_entry_from_path(album->path);
    if (album->repo == 0) {
        /* Could not find a repo -- this is probably not a album */
        return 0;
    }

	album->songs = malloc(sizeof(struct song_list));
	album->songs->num = 0;
	album->songs->entries = malloc(MAX_ALBUM_SONGS * sizeof(void*));

	return album;
}

void
album_load_songs(
    struct gitalbum *album, 
    struct song_list *songs) 
{ 
    struct dirent *dp; 
    DIR *dirp = opendir(album->path); 

    while(dirp) { 
        if ((dp = readdir(dirp)) != NULL) { 
            for (int i=0; i<songs->num; i++) { 
                if(strcmp(songs->entries[i]->slug, dp->d_name) == 0) {
                    album->songs->entries[album->songs->num++] = 
						songs->entries[i];
                }
            }
        } else {
            closedir(dirp);
            break;
        }
    }
}

int 
album_has_song(struct gitalbum *album, struct gitsong *song) 
{
    for (int i=0; i < album->songs->num; i++) {
        if(song == album->songs->entries[i]) {
            return 1;
        }
    }
    return 0;
}

struct album_list *
album_list_from_dir(char *album_dir) 
{
	struct album_list *albums = malloc(sizeof(struct album_list));
    albums->num = 0;
    albums->entries = malloc(MAX_ALBUMS * sizeof(void*));

    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(album_dir);
    while (dirp) {
        if ((dp = readdir(dirp)) != NULL) {
            if (dp->d_name[0] == '.' || 0 == strchr(dp->d_name, '\n')) {continue; }

			struct gitalbum *album = album_create(dp->d_name, album_dir);
			if ( album != 0 ) {
				albums->entries[albums->num++] = album;
			} else {
				fprintf(stderr, "\'%s\' is not an album\n", dp->d_name);
			}
                

        } else {
            closedir(dirp);
            break;
        }
    }

	return albums;
}

struct song_list *
album_find_exclusions(struct album_list *albums,
					  struct song_list *songs) 
{
    /* Loop over all songs, look for them in all albums
     * If a song is not found in any album, it is an orphan
     */
    struct song_list *orphans = malloc(sizeof(struct song_list));
    orphans->num = 0;
    orphans->entries = malloc(MAX_SONGS * sizeof(void*));

    for(int si=0; si<songs->num; si++) {
        int found = 0;
        for (int ai=0; ai<albums->num; ai++) {
            if (album_has_song(albums->entries[ai], 
                               songs->entries[si])) {
                found = 1;
                break;
            }
        }

        if (found == 0) {
            /* If we reach this point, no song matched */
            orphans->entries[orphans->num++] = songs->entries[si];
        }
    }

    return orphans;
}
