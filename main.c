#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define GITSONG_DIR "/Volumes/Audio/Workspace/songs"
#define GITALBUM_DIR "/Volumes/Audio/Workspace/album"
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
	struct gitsong* songs;
	int num_songs;
};

/*
int find_item_in_dir(char* item_name, char* path) {
	DIR* dirp;

	struct dirent* dp;

	dirp = opendir(".");
	while (dirp) {
		if ((dp = readdir(dirp)) != NULL) {
			if (strcmp(dp->d_name, item_name) == 0) {
				closedir(dirp);
				return 1;
			}
		} else {
			closedir(dirp);
			return 0;
		}
	}

	return -1;
} */

int load_songs(struct gitsong **songs, char *song_dir) {
	int num_loaded = 0;

	DIR *dirp;
	struct dirent *dp;

	dirp = opendir(song_dir);
	while (dirp) {
		if ((dp = readdir(dirp)) != NULL) {
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
			struct gitalbum *album = malloc(sizeof(struct gitalbum));

			album->slug = strdup(dp->d_name);
			album->path = malloc(strlen(album_dir) + 1 + strlen(dp->d_name) + 1);
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
	int num_loaded = 0;

	struct dirent *dp;
	DIR *dirp = opendir(album->path);
	while(dirp) {
		if ((dp = readdir(dirp)) != NULL) {
			for (int i=0; int i<num_songs; i++) {
				if(strcmp(songs[i]->slug, dp->d_name) == 0) {
					album->songs[num_loaded] = songs[i];
					num_loaded++;
				}
			}
		}
	}

	return num_loaded;
}



int 
find_orphans(struct gitsong **orphans, 
			 struct gitsong **songs, int num_songs,
			 struct gitalbum **albums, int num_albums) 
{
	/* hacker man code goes here */
	int num_found = 0;
	return num_found;
}

int main(int argc, char* argv[]) {
	struct gitsong  **songs = malloc(MAX_SONGS * sizeof(void*));
	struct gitalbum **albums = malloc(MAX_ALBUMS * sizeof(void*));

	int num_songs = load_songs(songs, GITSONG_DIR);
	int num_albums = load_albums(albums, GITALBUM_DIR);
	for(int i=0; i<num_albums; i++) {
		load_album_songs(albums[i], songs, num_songs);
	}

	printf("%s\n", albums[1]->songs[1]->slug);

	/*
	struct gitsong **orphans = malloc(MAX_SONGS * sizeof(void*));
	int num_orphans = find_orphans(orphans, songs, num_songs, albums, num_albums);
	if (num_orphans > 0) {
		for (int i=0; i<num_orphans; i++) {
			printf("%s\n", orphans[i]);
		}
	}
	*/


	
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
