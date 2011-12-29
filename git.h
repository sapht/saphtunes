#ifndef _GIT__H_
#define _GIT__H_

struct git_repo {
	char* git_origin;
    char* git_config_path;
	char* path;
};

int parse_git_config(struct git_repo *repo);
#endif
