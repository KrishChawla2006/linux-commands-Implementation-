#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

static int list_directory(const char *path, int show_hidden) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror(path);
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }

        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) != 0) {
        perror(path);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int show_hidden = 0;
    const char *path = ".";
    int path_set = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            show_hidden = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "my_ls: unsupported option: %s\n", argv[i]);
            return 1;
        } else if (!path_set) {
            path = argv[i];
            path_set = 1;
        } else {
            fprintf(stderr, "Usage: %s [-a] [DIRECTORY]\n", argv[0]);
            return 1;
        }
    }

    struct stat info;

    if (stat(path, &info) != 0) {
        perror(path);
        return 1;
    }

    if (S_ISDIR(info.st_mode)) {
        return list_directory(path, show_hidden);
    }

    printf("%s\n", path);
    return 0;
}