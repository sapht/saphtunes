#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define GITSONG_DIR "test/songs"
#define GITALBUM_DIR "test/album"
#define MAX_SONGS 4096
#define MAX_ALBUMS 512
#define MAX_ALBUM_SONGS 128

struct gitsong {
	char *path;
	char *slug;
	char *remote;
};

struct gitalbum {
	char *path;
	char *slug;
	struct gitsong** songs;
	int num_songs;
};

int load_songs(struct gitsong **songs, char *song_dir) {
	int num_loaded = 0;

	DIR *dirp;
	struct dirent *dp;

	dirp = opendir(song_dir);
	while (dirp) {
		if ((dp = readdir(dirp)) != NULL) {
			if (dp->d_name[0] == '.') {continue; }
			struct gitsong *song = malloc(sizeof(struct gitsong));

			song->slug = strdup(dp->d_name);
			song->remote = song->slug;
			song->path = malloc(strlen(song_dir) + 1 + strlen(dp->d_name) + 1);
			sprintf(song->path, "%s/%s", song_dir, dp->d_name);
			/*printf("%d:%d:%d\n", strlen(song->slug), strlen(song_dir), strlen(song->path));*/

			songs[num_loaded] = song;
			num_loaded++;
		} else {
			closedir(dirp);
			break;
		}
	}

	return num_loaded;
}

int load_albums(struct gitalbum **albums, char *album_dir) {
	int num_loaded = 0;

	DIR *dirp;
	struct dirent *dp;

	dirp = opendir(album_dir);
	while (dirp) {
		if ((dp = readdir(dirp)) != NULL) {
			if (dp->d_name[0] == '.') {continue; }
			struct gitalbum *album = malloc(sizeof(struct gitalbum));

			album->slug = strdup(dp->d_name);
			album->path = malloc(strlen(album_dir) + 1 + strlen(dp->d_name) + 1);
			album->songs = malloc(MAX_ALBUM_SONGS * sizeof(void*));
			album->num_songs = 0;

			sprintf(album->path, "%s/%s", album_dir, dp->d_name);

			albums[num_loaded] = album;
			num_loaded++;
		} else {
			closedir(dirp);
			break;
		}
	}

	return num_loaded;
}

int load_album_songs(struct gitalbum *album, struct gitsong **songs, int num_songs) {
	struct dirent *dp;
	DIR *dirp = opendir(album->path);
	while(dirp) {
		if ((dp = readdir(dirp)) != NULL) {
			for (int i=0; i<num_songs; i++) {
				if(strcmp(songs[i]->slug, dp->d_name) == 0) {
					album->songs[album->num_songs] = songs[i];
					album->num_songs++;
				}
			}
		} else {
			closedir(dirp);
			break;
		}
	}

	return album->num_songs;
}

int find_orphans(struct gitsong **orphans, 
				 struct gitsong **songs, int num_songs,
				 struct gitalbum **albums, int num_albums) 
{
	/* Loop over all songs, look for them in all albums
	 * If a song is not found in any album, it is an orphan
	 */
	int num_orphans = 0;

	for(int si=0; si<num_songs; si++) {
		for (int ai=0; ai<num_albums; ai++) {
			for (int asi=0; asi < albums[ai]->num_songs; asi++) {
				if(songs[si] == albums[ai]->songs[asi]) {
					continue 3;
				}
			}
		}

		/* If we reach this point, no song matched */
		orphans[num_orphans] = songs[si];
		num_orphans++;
	}

	return num_orphans;
}

int main(int argc, char* argv[]) {
	struct gitsong  **songs = malloc(MAX_SONGS * sizeof(void*));
	struct gitalbum **albums = malloc(MAX_ALBUMS * sizeof(void*));

	int num_songs = load_songs(songs, GITSONG_DIR);
	int num_albums = load_albums(albums, GITALBUM_DIR);
	printf("albums:%d\tsongs:%d\n", num_albums, num_songs);
	for(int i=0; i<num_albums; i++) {
		load_album_songs(albums[i], songs, num_songs);
	}

	/*
	printf("%s\n", albums[1]->songs[0]->slug); */

	struct gitsong **orphans = malloc(MAX_SONGS * sizeof(void*));
	int num_orphans = find_orphans(orphans, songs, num_songs, albums, num_albums);
	if (num_orphans > 0) {
		for (int i=0; i<num_orphans; i++) {
			printf("%s\n", orphans[i]);
		}
	}


	
	/*printf ("Hello %s", song.title);


	char* name = "main.c";
	switch(find_item_in_dir(name, ".")) {
		case 1:
			printf("Found");
			break;
		case 0:
			printf("Not found");
			break;
		case -1:
			printf("Error");
			break;
	}
	*/

	return 0;
}
