#include "git.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

struct git_config *
git_config_from_file(char* config_path)
{
	struct git_config *config = malloc(sizeof(struct git_config));
	config->path = strdup(config_path);

	FILE *fp = fopen(config->path, "rb");
	if (fp == 0) { 
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	config->filesize = ftell(fp);
	rewind(fp);
	config->data = malloc(config->filesize + 1);
	fread(config->data, 1, config->filesize, fp);
	config->data[config->filesize] = '\0';
	fclose(fp);

	return config;
}

char *
git_origin_from_config(struct git_config *config) 
{
	char *begin_block = strstr(config->data, "[remote \"origin\"]");
	if ( 0 == begin_block ) {
		return "";
	} else {
		begin_block++;
		char *end_block = strstr(begin_block, "[");
		if (end_block != 0) { *end_block = 0; }

		char *url_begin = strstr(begin_block, "url = ");
		if (url_begin == 0) {
			return "";
		} else {
			url_begin += strlen("url = ");
			char *url_end = strchr(url_begin, '\n');
			*url_end = 0;
			char *r = strdup(url_begin);
			free(config->data);
			return r;
		}
	}
}

struct git_entry *
git_entry_from_path(char* repo_path)
{
	struct git_entry *entry = malloc(sizeof(struct git_entry));

	char* config_path = malloc(strlen(repo_path) + strlen(".git/config") + 2);
	sprintf(config_path, "%s/%s", repo_path, ".git/config");

	struct git_config *config = git_config_from_file(config_path);
	if ( 0 == config ) {
		return 0;
	}
	entry->path = repo_path;
	entry->origin = git_origin_from_config(config);

	return entry;
}
