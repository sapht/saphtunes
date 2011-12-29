#include "git.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv) {
	struct git_config *config = git_config_from_file("git_config");
	printf("%s\n", git_origin_from_config(config));
}
