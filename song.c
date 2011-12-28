#include "song.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

struct gitsong *song_create 
(char *slug, char *song_dir)
{
	struct gitsong *song = malloc(sizeof(struct gitsong));

	song->slug = strdup(slug);
	song->remote = song->slug;
	song->path = malloc(2 + strlen(song_dir) + strlen(slug));
	sprintf(song->path, "%s/%s", song_dir, slug);

	return song;
}

struct song_list *
song_list_from_dir(char *song_dir) {
	struct song_list *song_list = malloc(sizeof(struct song_list));
    song_list->num = 0;
    song_list->entries = malloc(MAX_SONGS * sizeof(void*));

    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(song_dir);
    while (dirp) {
        if ((dp = readdir(dirp)) != 0) {
            if (dp->d_name[0] == '.') {continue; }

            song_list->entries[song_list->num++] = song_create(dp->d_name, song_dir);
        } else {
            closedir(dirp);
            break;
        }
    }

    return song_list;
}


struct song_list *
song_list_not_cloned(struct song_list *songs, char *git_dir) {
	/* Look at all items in git_dir and try to find their clones in song_list
	 * If no clone is found, add it to the response
	 */

	struct song_list *not_cloned = malloc(sizeof(struct song_list));
	not_cloned->num = 0;
	not_cloned->entries = malloc(MAX_SONGS * sizeof(void*));

	DIR *dirp;
	struct dirent *dp;
	while(dirp) {
		if ((dp = readdir(dirp)) != 0) {
			if (dp->d_name == '.') { continue; }

			int found = 0;
			for(int i=0; i<songs->num; i++) {
				if(strcmp(songs->entries[i]->remote, dp->d_name) == 0) {
					// XXX: This will not work since d_name has .git
					 found = 1;
					 break;
				}
			}

			if (found == 0) {
				not_cloned->entries[not_cloned->num++] = songs
			}
		} else {
			closedir(dirp);
			break;
		}
	}
}
