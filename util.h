#include <sys/types.h>
#include <dirent.h>

#ifndef _UTIL__H_
#define _UTIL__H_

#define NUM_ELEMENTS(a) ((sizeof a)/(sizeof a[0]))

struct dir_list {
    dirent **entries;
    int entries_len;
}
#endif
