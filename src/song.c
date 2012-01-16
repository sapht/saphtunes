#include <stdlib.h>
#include <stdio.h>
#include "song.h"
#include "util.h"
#include "cache.h"
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
    r->project_path = malloc(256 * sizeof(char)); 
    r->render_path  = malloc(256 * sizeof(char)); 
    sprintf(r->render_path,  "%s/%s.wav",    r->path, r->slug);
    r->render_path_len  = strlen(render_path);
    sprintf(r->project_path, "%s/%s.reason", r->path, r->slug);
    r->project_path_len = strlen(render_path);
    r->render_stat = song_render_analyze(r->render_path);

    return r;
}

void
song_list_render(struct song_list *song_list, char *render_script_path)
{
    struct song *song;
    struct {
        char **command;
        int command_num;
    } queue;
    queue.command_num = 0;
    queue.command = malloc(sizeof(void*) * song_list->len);

    for(int j=0; j<song_list->len; j++) {
        song = song_list->e[i];

        queue.command[queue.command_num] = malloc((
                strlen("osascript  xx xx")
              + strlen(render_script_path)
              + strlen(song->slug)
              + 1) * sizeof(char));

        queue.command[queue.command_num] = sprintf(
            "osascript %s \"%s\" \"%s.wav\"",
            render_script_path,
            song->project_path,
            song->slug
        );

        queue.command_num++; 
    }

    FILE *pipe;
    for(int i=0; i<queue.command_num; i++) {
        if(!(fpipe = popen(queue.command[i], "r"))) {
            perror("Problem with le pipe");
            exit(1);
        }
    }
}

struct song_render_stat
song_render_analyze(char *render_path)
{
    struct song_render_stat stat;
    char *cache_id = malloc(256);
    sprintf(cache_id, "render_%s", render_path);

    int render_mtime = get_mtime(render_path);
    if (!render_mtime) {
        printf("Render could not analyze: nonexistent: %s\n", render_path);
        stat.nullspace = -1;
        stat.clipping  = -1;
        stat.duration  = -1;
        return stat;
    }

    struct cache_entry *e = cache_get(cache_id, render_mtime);
    if (e) {
        return *((struct song_render_stat*) e->value);
    } else {
        printf("Render Analyze: loading info on %s from FS...\n", render_path);
    }

    FILE *fp = fopen(render_path, "rb");
    if(!fp) {
        fprintf(stderr, "Unknown error reading render of %s\n", render_path);
        exit(1);
    };

    /* Assume 16 bit depth, 48khz */

    short nullspace_num_sec_multiplier = 10;
    short clipping_num_sec_divisor     = 10;
    int sample_second = 44100 * 2 * 2; /* rate * channel * sample size */

    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    stat.duration = filesize / sample_second;

    fseek(fp, -sample_second * nullspace_num_sec_multiplier, SEEK_CUR);

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
    fseek(fp, -sample_second/clipping_num_sec_divisor, SEEK_CUR);
    while(fread(&ch1_val, 2, 1, fp)
       && fread(&ch2_val, 2, 1, fp)
    ) {
        mean = abs((ch1_val + ch2_val) / 2);
        if (mean > silence_threshold) { loudness++; }
    }

    stat.clipping = loudness > 0;

    cache_set(cache_id, render_mtime, &stat, sizeof(stat));

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
