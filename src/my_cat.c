#include <stdio.h>
#include <stdlib.h>

static int display_file(FILE *file) {
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (putchar(ch) == EOF) {
            perror("my_cat: output");
            return 1;
        }
    }

    if (ferror(file)) {
        perror("my_cat: read");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (argc == 1) {
        return display_file(stdin);
    }

    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");

        if (file == NULL) {
            perror(argv[i]);
            status = 1;
            continue;
        }

        if (display_file(file) != 0) {
            status = 1;
        }

        if (fclose(file) != 0) {
            perror(argv[i]);
            status = 1;
        }
    }

    return status;
}