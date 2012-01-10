#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include "git.h"
#include "util.h"
#include "cache.h"


int
git_load_generic(struct git_repo *repo, 
                 char *root, 
                 char *name)
{
    repo->path = malloc(255 * sizeof(char));
    sprintf(repo->path, "%s/%s", root, name);

    repo->config_path = malloc(255 * sizeof(char));
    sprintf(repo->config_path, "%s/.git/config", repo->path);

    repo->status = 0;

    return git_load_config(repo);
}

int
git_load_status(struct git_repo *repo)
{
    int cache_key_len = strlen(repo->path) + 2;
    char *cache_key = malloc(cache_key_len);
    sprintf(cache_key, "S%s", repo->path);
    cache_key[cache_key_len - 1] = '\0';

    int mtime_dir = get_mtime(repo->path);
    struct cache_entry *e = cache_get(cache_key, mtime_dir);
    if(e) {
        repo->status = strdup(e->value);
    } else {
        printf("Git status: open proc for %s...\n", repo->path);
        char *command = malloc(256);
        sprintf(command, "cd %s && git status --porcelain -s", repo->path);

        FILE *fpipe;

        if ( !(fpipe = popen(command, "r"))) {
            perror("Problem with le pipe");
            exit(1);
        }

        char fbuf[1024];
        memset(fbuf, 0, 1024);

        int data_len = (int) fread(fbuf, 1, 1024, fpipe);
        /*  data_len == 1 */
        /* sorry for ugliness */
        if(ferror(fpipe) && errno != 4) { 
            printf("IMPOSSIBL!\n");
            exit(1);
        }

        pclose(fpipe);

        repo->status = malloc(data_len + 1);
        memcpy(repo->status, fbuf, data_len);
        repo->status[data_len] = '\0';

        cache_set(cache_key, mtime_dir, repo->status, data_len + 1);
    }

    return 1;
}

int
git_load_config(struct git_repo *repo)
{
    int config_filesize;
    char* config_data;

    int config_mtime = get_mtime(repo->config_path);
    if (!config_mtime) {
        return 0;
    }

    char *cache_id = repo->config_path;
    struct cache_entry *ce = cache_get(cache_id, config_mtime);
    if (ce) {
        config_data = malloc(sizeof(char) * ce->value_len);
        memcpy(config_data, ce->value, ce->value_len);
    } else {
        printf("Loading config data %s from FS...\n", repo->config_path);
        FILE *fp = fopen(repo->config_path, "rb");
        if (fp == 0) {
            return 0;
        }

        /* Read entire data into buffer */
        fseek(fp, 0, SEEK_END);

        config_filesize = ftell(fp);
        config_data = malloc(config_filesize + 1);

        rewind(fp);

        fread(config_data, 1, config_filesize, fp);
        config_data[config_filesize] = '\0';

        fclose(fp);

        cache_set(cache_id, config_mtime, config_data, config_filesize+1);
    }


    /* Begin parsing */
	char *begin_block = strstr(config_data, "[remote \"origin\"]");
	if ( 0 != begin_block ) {
		begin_block++;
		char *end_block = strstr(begin_block, "[");
		if (end_block != 0) { *end_block = 0; }

		char *url_begin = strstr(begin_block, "url = ");

		if (url_begin != 0) {
			url_begin += strlen("url = ");
			char *url_end = strchr(url_begin, '\n');
			*url_end = 0;
			repo->origin = strdup(url_begin);
            /*printf("Found origin: %s\n", repo->origin);*/
            return 1;
		}
	}

    return 0;
}

int
git_repo_fill_submodules(struct git_repo *root_repo, char **paths)
{
    /* Find all submodules and return their count */

    int path_num = 0;

    char *submodule_dotfile_path = malloc(255);
    sprintf(submodule_dotfile_path, "%s/%s", root_repo->path, ".gitmodules");
    
    FILE *fp = fopen(submodule_dotfile_path, "rb");
    if(0 == fp) {
        /* There was no .gitmodule file. */
        return 0;
    } else {
        fseek(fp, 0, SEEK_END);
        int fsize = ftell(fp);
        rewind(fp);

        char *data = malloc(fsize + 1);
        fread(data, 1, fsize, fp);
        fclose(fp);

        data[fsize] = '\0';

        char *url_begin = data;
        while((url_begin = strstr(url_begin, "url = "))) {
            url_begin += strlen("url = ");
            char *url_end = strchr(url_begin, '\n');
            *url_end = '\0';

            paths[path_num++] = strdup(url_begin);
            *url_end = '\n';
        }
    }

    return path_num;
}
