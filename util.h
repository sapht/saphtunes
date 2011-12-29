#include <sys/types.h>
#include <dirent.h>

#ifndef _UTIL__H_
#define _UTIL__H_

#define NUM_ELEMENTS(a) ((sizeof a)/(sizeof a[0]))

struct dirent_list {
    struct dirent **e;
    int len;
};

struct dirent_list * dir_read_all(char *dir);
#endif
