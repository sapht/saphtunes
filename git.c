#include "git.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


int
git_load_generic(struct git_repo *repo, 
                 char *root, 
                 char *name)
{

    repo->path = malloc(255 * sizeof(char));
    sprintf(repo->path, "%s/%s", root, name);

    repo->git_config_path = malloc(255 * sizeof(char));
    sprintf(repo->git_config_path, "%s/.git/config", repo->path);

    return git_load_config(repo);
}

int
git_load_config(struct git_repo *repo)
{
    int config_filesize;
    char* config_data;

	FILE *fp = fopen(repo->git_config_path, "rb");
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
    free(fp);

    /* Begin parsing */
	char *begin_block = strstr(config_data, "[remote \"origin\"]");
	if ( 0 == begin_block ) {
		return 0;
	} else {
		begin_block++;
		char *end_block = strstr(begin_block, "[");
		if (end_block != 0) { *end_block = 0; }

		char *url_begin = strstr(begin_block, "url = ");
		if (url_begin == 0) {
			return 0;
		} else {
			url_begin += strlen("url = ");
			char *url_end = strchr(url_begin, '\n');
			*url_end = 0;
			repo->git_origin = strdup(url_begin);
            return 1;
		}
	}
}
