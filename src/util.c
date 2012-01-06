#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include <sys/types.h>
#include <sys/stat.h>

struct dirent_list
dir_read_all(char *dir)
{
    struct dirent_list r;
    r.e = malloc(DIRENT_MAX * sizeof(struct dirent));
    r.len = 0;

    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dir);
    if(0 == dirp) {
        fprintf(stderr, "Could not read %s\n", dir);
        exit(1);
    }
    while (dirp) {
        if ((dp = readdir(dirp)) != NULL) {
            /* Ignore bogus files */
            if (dp->d_name[0] == '.' || 0 != strchr(dp->d_name, '\r')) {
                continue;
            }

            r.e[r.len++] = *dp;
        } else {
            closedir(dirp);
            break;
        }
    }
    r.e = realloc(r.e, r.len * sizeof(struct dirent));
    return r;
}

time_t get_mtime(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        return 0;
    }

    return statbuf.st_mtime;
}

