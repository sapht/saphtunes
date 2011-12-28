#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "album.h"



int main(int argc, char* argv[]) {
    struct song_list  *songs  = song_list_from_dir(SONG_DIR);
	struct album_list *albums = album_list_from_dir(ALBUM_DIR);

	for(int i=0; i<albums->num; i++) {
		printf("loading songs for index %d=%s\n", i, albums->entries[i]->slug);
		album_load_songs(albums->entries[i], songs);
	}

	for(int i=0; i<albums->num; i++) {
		printf("album:%s\n", albums->entries[i]->slug);
	}
	for(int i=0; i<songs->num; i++) {
		printf("song:%s\n", songs->entries[i]->slug);
	}
	return 0;



    /*struct song_list *orphans = album_find_exclusions(albums, songs);
    if (orphans->num > 0) {
        for (int i=0; i<orphans->num; i++) {
            printf("%s\n", orphans->entries[i]->slug);
        }
    }

    return 0;*/
}
