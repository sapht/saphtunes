#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include "cache.h"

uint32_t
hash_fn (void *data, uint32_t len)
{
    uint32_t h, x;
    char *dd = (char*)data;

    for (h = x = 0; x<len; x++) {
        h += dd[x];
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

int
cache_load(char *dump_path)
{
    FILE *fp = fopen(dump_path, "rb");
    if(!fp) {
        /* There's no dump file */
        return 0;
    }

    /*fseek(fp, 0, SEEK_END);*/
    /*int filesize = ftell(fp);*/
    /*rewind(fp);*/

    st.cache_num = 0;
    struct cache_entry *c = &st.cache[0];
    while(
        fread(&c->key_len,   sizeof(int), 1, fp) &&
        fread(&c->value_len, sizeof(int), 1, fp) &&
        fread(&c->timestamp, sizeof(int), 1, fp)
    ) {
        c->key   = malloc(c->key_len);
        c->value = malloc(c->value_len);

        fread(c->key,   c->key_len,   sizeof(void*), fp);
        fread(c->value, c->value_len, sizeof(void*), fp);


        uint32_t hash_offset = hash_fn(c->key, c->key_len) & 2047;
        struct hash *he = st.cachetable[hash_offset];

        do {
            if (!he || !strcmp(he->key, c->key)) {
                he = malloc(sizeof(struct hash));
                he->key = strdup(c->key);
                he->value = c;
                break;
            }
        } while ((he = he->next));

        c = &st.cache[st.cache_num++];
    }

    /* Cache was loaded */
    return 1;
}

int
cache_dump(char *dump_path)
{
    FILE *fp = fopen(dump_path, "w");

    if(!fp) {
        /* Could not dump! */
        return 0;
    }

    for(int i = 0; i<st.cache_num; i++ ) {
        fwrite(&st.cache[i],       sizeof(int) * 3,       1, fp);
        fwrite(st.cache[i].key,    st.cache[i].key_len,   1, fp);
        fwrite(st.cache[i].value,  st.cache[i].value_len, 1, fp);
    }

    fclose(fp);

    return 1;
}


void *
cache_get(char *id, int ts) {
    uint32_t hash_offset = hash_fn(id, strlen(id)) & 2047;

    struct hash *e = st.cachetable[hash_offset];

    if(e) {
        /* There's at least 1 value on this offset */
        do {
            if(!strcmp(e->key, id)) {
                if(ts <= e->value->timestamp) {
                    /* The timestamp is fresh */
                    return e->value;
                }
            }
        } while((e = e->next));
    }

    return 0;
}

int
cache_set(char *id, int ts, void *data, int data_len) {
    uint32_t hash_offset = hash_fn(id, strlen(id)) & 2047;
    struct hash *e = st.cachetable[hash_offset];

    do {
        if (!e || !strcmp(e->key, id)) {
            struct cache_entry ce;
            ce.key_len = strlen(id);
            ce.value_len = data_len;
            ce.key   = strdup(id);

            ce.value = malloc(ce.value_len);
            memcpy(ce.value, data, data_len);

            st.cache[st.cache_num++] = ce;

            e = malloc(sizeof(struct hash));
            e->key = strdup(id);
            e->value = &ce;
            return 1;
        }
    } while ((e = e->next));

    fprintf(stderr, "could not set cache entry %s", id);
    exit(1);
    return 0;
}
