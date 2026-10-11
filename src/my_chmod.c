
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: my_chmod MODE FILE\n");
        return 1;
    }

    char *endptr;
    errno = 0;

    long mode = strtol(argv[1], &endptr, 8);

    if (errno != 0 || *argv[1] == '\0' || *endptr != '\0' ||
        mode < 0 || mode > 07777) {
        fprintf(stderr, "my_chmod: invalid mode: %s\n", argv[1]);
        return 1;
    }

    if (chmod(argv[2], (mode_t)mode) != 0) {
        perror("my_chmod");
        return 1;
    }

    return 0;
}
