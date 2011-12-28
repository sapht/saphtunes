#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


int load_songs(struct song_list **songs, char *song_dir) {
    songs->num = 0;
    songs->entries = malloc(MAX_SONGS * sizeof(void*));

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

int album_load_songs(struct gitalbum *album) {
    album->songs->num = 0;
}
struct gitalbum *album_create(char* slug, char* path) {
    struct gitalbum *album = malloc(sizeof(struct gitalbum));

    album->slug = strdup(dp->d_name);
    album->path = malloc(strlen(album_dir) + 1 + strlen(dp->d_name) + 1);
    sprintf(album->path, "%s/%s", album_dir, dp->d_name);

    album_load_songs(album)
}

void album_list_load(struct album_list *albums, char *album_dir) {
    albums->num = 0;
    albums->entries = malloc(MAX_ALBUMS * sizeof(void*));

    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(album_dir);
    while (dirp) {
        if ((dp = readdir(dirp)) != NULL) {
            if (dp->d_name[0] == '.') {continue; }

            albums->entries[albums->num++] = 
                album_create(dp->d_name, album_dir);

        } else {
            closedir(dirp);
            break;
        }
    }
}

int 
album_load_songs(
    struct gitalbum *album, 
    struct song_list *songs) 
{ 
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

int album_has_song(struct gitalbum *album, struct gitsong *song) {
    for (int i=0; i < album->num_songs; i++) {
        if(song == album->songs[i]) {
            return 1;
        }
    }
    return 0;
}

struct song_list *song_find_orphans(
                 struct song_list *songs,
                 struct album_list *albums) 
{
    /* Loop over all songs, look for them in all albums
     * If a song is not found in any album, it is an orphan
     */
    struct song_list *orphans;
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
            orphans->entries[orphans->num++] = songs[si];
        }
    }

    return orphans;
}

int main(int argc, char* argv[]) {
    struct song_list  *songs;  
    struct album_list *albums; 

    song_list_load  (songs, SONG_DIR);
    album_list_load (albums, ALBUM_DIR);

    for(int i=0; i<songs->num; i++) {
        album_load_songs(albums->entries[i], songs);
    }

    struct gitsong **orphans = song_find_orphans(songs, albums);
    if (orphan->num > 0) {
        for (int i=0; i<orphans->num; i++) {
            printf("%s\n", orphans->entries[i]->slug);
        }
    }

    return 0;
}
