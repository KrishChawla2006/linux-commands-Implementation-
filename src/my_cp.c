#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s SOURCE DESTINATION\n", argv[0]);
        return 1;
    }

    struct stat src_stat, dst_stat;

    if (stat(argv[1], &src_stat) != 0) {
        perror(argv[1]);
        return 1;
    }

    if (!S_ISREG(src_stat.st_mode)) {
        fprintf(stderr, "my_cp: source must be a regular file\n");
        return 1;
    }

    if (stat(argv[2], &dst_stat) == 0) {
        if (src_stat.st_dev == dst_stat.st_dev &&
            src_stat.st_ino == dst_stat.st_ino) {
            fprintf(stderr, "my_cp: source and destination are the same file\n");
            return 1;
        }
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        perror(argv[1]);
        return 1;
    }

    FILE *dst = fopen(argv[2], "wb");
    if (dst == NULL) {
        perror(argv[2]);
        fclose(src);
        return 1;
    }

    int ch;
    int status = 0;

    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            perror("my_cp: write");
            status = 1;
            break;
        }
    }

    if (ferror(src)) {
        perror("my_cp: read");
        status = 1;
    }

    if (fclose(src) != 0) {
        perror("my_cp: close source");
        status = 1;
    }

    if (fclose(dst) != 0) {
        perror("my_cp: close destination");
        status = 1;
    }

    return status;
}