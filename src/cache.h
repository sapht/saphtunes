#ifndef _ST_CACHE__H_
#define _ST_CACHE__H_
#include <stdint.h>

struct cache_entry {
    int key_len;
    int value_len;
    int timestamp;
    char *key;
    void *value;
};

struct hash {
    char *key;

    struct cache_entry *value;
    struct hash *next;
};

uint32_t hash_fn (void *data, uint32_t len);
int cache_load(char *dump_path);
int cache_dump(char *dump_path);
void * cache_get(char *id, int ts);
int cache_set(char *id, int ts, void *data, int data_len);
#endif
