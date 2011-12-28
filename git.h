struct git_entry {
	char* origin;
	char* path;
};

struct git_config {
	char* path;
	char* data;
	long filesize;
};

struct git_entry *git_entry_from_path(char* repo_path);
struct git_config * git_config_from_file(char* config_path);
char * git_origin_from_config(struct git_config *config);
