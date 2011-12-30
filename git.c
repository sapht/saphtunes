#include "git.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


int
git_load_generic(struct git_repo *repo, 
                 char *root, 
                 char *name)
{

    repo->path = malloc(255 * sizeof(char));
    sprintf(repo->path, "%s/%s", root, name);

    repo->config_path = malloc(255 * sizeof(char));
    sprintf(repo->config_path, "%s/.git/config", repo->path);

    printf("Git Path %s has config %s\n", repo->path, repo->config_path);

    return git_load_config(repo);
}

int
git_load_config(struct git_repo *repo)
{
    int config_filesize;
    char* config_data;

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
			repo->origin = strdup(url_begin);
            printf("Found origin: %s\n", repo->origin);
            return 1;
		}
	}
}

int 
git_repo_has_submodule(struct git_repo *root_repo, 
                       char *expected_origin)
{
    /* TODO: this process is slow and uneccesary
     * parse .gitmodules instead
     */
    struct dirent_list files = dir_read_all(root_repo->path);

    struct git_repo *subrepo = malloc(sizeof(struct git_repo));
    char *subrepo_config_path = malloc(255 * sizeof(char));
    struct stat subrepo_stat;
    int i = 0;

    /*printf("Looking at %s, %d files\n", root_repo->path, files.len);*/
    for(; i<files.len; i++) {
        sprintf(subrepo_config_path, 
                "%s/%s/.git/config", 
                root_repo->path, 
                files.e[i].d_name);


        if (0 == stat(subrepo_config_path, &subrepo_stat)) {
            /* This is probably a subrepo. */
            git_load_generic(subrepo, root_repo->path, files.e[i].d_name);
            /*printf("path=%s, Origin=%s, expected=%s", subrepo->path, subrepo->origin, expected_origin);*/
            if(subrepo->origin == expected_origin) {
                /* Found a matching subrepo. */
                /*printf("Found a matching subrepo\n");*/
                return 1;
            }
        }
    }

    return 0;
}
