#ifndef _GIT__H_
#define _GIT__H_

struct git_repo {
	char* origin;
    char* config_path;
	char* path;
};

int git_load_config(struct git_repo *repo);
int git_load_generic(struct git_repo *repo, char *root, char *path);
int git_repo_has_submodule(struct git_repo *repo, char *expected_origin);
#endif
