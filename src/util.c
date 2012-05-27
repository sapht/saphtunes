#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include <sys/types.h>
#include <sys/stat.h>


/* A dir filter returns 1 for items to include */
int dir_filter_none(struct dirent *dp) {
    return 1;
}

int dir_filter_is_dir(struct dirent *dp) {
    return DT_DIR==dp->d_type;
}

int dir_filter_is_symlink(struct dirent *dp) {
    return DT_LNK==dp->d_type;
}
int dp_name_parse_order(char *str) {
    /* TODO */
    int pos = 0;
    char *s = str;
    for(;;s++) {
        if(*s >= '0' && *s <= '9') {
            pos *= 10;
            pos += *s - '0';
            continue;
        } else if(*s == '.') { 
            break;
        }

        return 0;
    }
    fprintf(stderr, "we has a winrr!!! %d, %s\n", pos, str);
    return pos;
}

int dp_cmp(struct dirent *dp1, struct dirent *dp2) {
    int o1, o2;
    o1 = dp_name_parse_order(dp1->d_name);
    o2 = dp_name_parse_order(dp2->d_name);
    if(o1 && o2) {
        /*printf("%s(%d) or %s(%d)?\n", dp1->d_name, o1, dp2->d_name, o2);*/
        if(o1 > o2) {
            /*printf("%s after %s\n", dp1->d_name, dp2->d_name);*/
            return 1;
        }
        if(o1 < o2) {
            /*printf("%s after %s\n", dp2->d_name, dp1->d_name);*/
            return -1;
        }
        /*printf("Neither!\n");*/
        return 0;
    }

    return 0;

    /*if(o1) { return -1; }*/
    /*if(o2) { return 1; }*/

    /*return strcmp( dp1->d_name, dp2->d_name );*/
}

void dirent_list_sort(struct dirent_list *dl) {
    qsort(  dl->e,
            dl->len,
            sizeof(struct dirent),
            dp_cmp);
}


struct dirent_list
dir_read_all(char *dir, int (*filter)(struct dirent*))
{
    struct dirent_list r;
    dirent_list_init(&r);

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
            if (!filter(dp) || dp->d_name[0] == '.' || 0 != strchr(dp->d_name, '\r')) {
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

void
dirent_list_init(struct dirent_list *r)
{
    r->e = malloc(DIRENT_MAX * sizeof(struct dirent));
    r->len = 0;
}

void
dirent_list_free(struct dirent_list *dl)
{
    free(dl->e);
    free(dl);
}

time_t get_mtime(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        return 0;
    }

    return statbuf.st_mtime;
}

