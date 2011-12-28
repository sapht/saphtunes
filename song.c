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
    song->path = malloc(2 + strlen(song_dir) + strlen(slug));
    sprintf(song->path, "%s/%s", song_dir, slug);

    song->repo = git_entry_from_path(song->path);
    if (song->repo == 0) {
        /* Could not find a repo -- this is probably not a song */
        return 0;
    }

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

            struct gitsong *song = song_create(dp->d_name, song_dir);
            if (song != 0) {
                song_list->entries[song_list->num++] = song;
            }
        } else {
            closedir(dirp);
            break;
        }
    }

    return song_list;
}


int
song_repos_not_cloned(char **not_cloned, 
                     struct song_list *songs, 
                     char *git_dir) 
{
    /* Look at all items in git_dir and try to find their clones in song_list
     * If no clone is found, add it to the response
     */
    int num_not_cloned = 0;

    DIR *dirp = opendir(git_dir);
    struct dirent *dp;

    while(dirp) {
        if ((dp = readdir(dirp)) != 0) {
            if (dp->d_name[0] == '.') { continue; }
            char* path = malloc(strlen(git_dir) + strlen(dp->d_name) + 2);
            sprintf(path, "%s/%s", git_dir, dp->d_name);

            int found = 0;
            for(int i=0; i<songs->num; i++) {
                if(strcmp(songs->entries[i]->repo->origin, path) == 0) {
                     found = 1;
                     break;
                }
            }

            if (found == 0) {
                not_cloned[num_not_cloned] = strdup(dp->d_name);
                num_not_cloned++;
            }
        } else {
            closedir(dirp);
            break;
        }
    }

    return num_not_cloned;
}
