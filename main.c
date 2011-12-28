#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "album.h"



int main(int argc, char** argv) {
    struct song_list  *songs  = song_list_from_dir(SONG_DIR);
	struct album_list *albums = album_list_from_dir(ALBUM_DIR);

	if (songs == 0) {
		fprintf(stderr, "Unable to create songs\n");
		return 1;
	}

	if (albums == 0) {
		fprintf(stderr, "Unable to create albums\n");
		return 1;
	}

	for(int i=0; i<albums->num; i++) {
		/*printf("loading songs for index %d=%s\n", i, albums->entries[i]->slug);*/
		album_load_songs(albums->entries[i], songs);
	}

	if (argc < 2) {
		/* This means interactive mode */
		/* ncurses etc... */
	} else {
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
	}

	return 0;
	

	/*for(int i=0; i<albums->num; i++) {
		printf("album:%s\n", albums->entries[i]->slug);
	}
	for(int i=0; i<songs->num; i++) {
		printf("song:%s\n", songs->entries[i]->slug);
	}
	return 0;*/






	return 0;
}
