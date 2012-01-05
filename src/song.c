#include <stdlib.h>
#include <stdio.h>
#include "song.h"
#include "util.h"
#include <string.h>

int
song_qsort(struct song **a,
           struct song **b)
{
    return strcmp((*a)->slug, (*b)->slug);
}

struct song *
song_create(char *root, char *name)
{
    struct song *r = malloc(sizeof(struct song));

    int could_load = git_load_generic(&r->git, root, name);
    if(!could_load) {
        printf("Could not load %s\n", name);
        exit(1);
    }

    r->slug = name;
    git_load_status(&r->git);
    r->path = r->git.path;
    r->render_path = malloc(256 * sizeof(char));

    sprintf(r->render_path, "%s/%s.wav",
            r->path,
            r->slug);

    /*printf("Path to render: %s\n", r->render_path);*/
    r->render_stat = song_render_analyze(r->render_path);

    return r;
}

struct song_render_stat
song_render_analyze(char *render_path)
{
    struct song_render_stat stat;
    FILE *fp = fopen(render_path, "rb");
    if (!fp) {
        printf("Could not detect analyze of %s\n", render_path);
        stat.nullspace = -1;
        stat.clipping  = -1;
        return stat;
    }

    /* Assume 16 bit depth, 48khz */

    short nullspace_num_sec_multiplier = 10;
    short clipping_num_sec_divisor     = 10;

    fseek(fp, 0, SEEK_END);
    fseek(fp, -48000 * 2 * 2 * nullspace_num_sec_multiplier, SEEK_CUR);

    short ch1_val;
    short ch2_val;
    short mean;
    short silence_threshold = 5;

    int loudness = 0;

    /* Check for nullspace */
    while(fread(&ch1_val, 2, 1, fp)
       && fread(&ch2_val, 2, 1, fp)
    ) {
        mean = abs((ch1_val + ch2_val) / 2);
        if (mean > silence_threshold) { loudness++; }
    }

    stat.nullspace = !loudness;

    loudness = 0;
    fseek(fp, (-48000 * 2 * 2)/clipping_num_sec_divisor, SEEK_CUR);
    while(fread(&ch1_val, 2, 1, fp)
       && fread(&ch2_val, 2, 1, fp)
    ) {
        mean = abs((ch1_val + ch2_val) / 2);
        if (mean > silence_threshold) { loudness++; }
    }

    stat.clipping = loudness > 0;

    return stat;
}

int
songs_load_dir(char* dir, struct song_list *song_list) {
    struct dirent_list files = dir_read_all(dir);

    song_list->e = malloc(SONG_MAX_NUM * sizeof(void*));
    song_list->len = files.len;

    /*fprintf(stderr, "Length: %d (dir: %s)\n", files.len, dir);*/

    for(int i=0; i<files.len; i++) {
        song_list->e[i] = song_create(
                dir, 
                files.e[i].d_name);
    }

    return song_list->len;
}

int
song_repos_not_cloned(char *git_dir,
                      struct song_list *song_list,
                      char **not_cloned)
{
    return 0;
}
