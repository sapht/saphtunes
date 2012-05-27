#include <sys/types.h>
#include <dirent.h>

#ifndef _UTIL__H_
#define _UTIL__H_

#define DIRENT_MAX 4096
#define NUM_ELEMENTS(a) ((sizeof a)/(sizeof a[0]))
#define PATH_MAX_LEN 255

struct dirent_list {
    struct dirent *e;
    int len;
};

struct dirent_list dir_read_all(char *dir, 
    int (*filter)(struct dirent*));
int dir_filter_is_dir(struct dirent *dp);
int dir_filter_none(struct dirent *dp);
int dir_filter_is_symlink(struct dirent *dp);
time_t get_mtime(const char *path);
void dirent_list_free(struct dirent_list *dl);
void dirent_list_init(struct dirent_list *r);
void dirent_list_sort(struct dirent_list *dl);

#endif
