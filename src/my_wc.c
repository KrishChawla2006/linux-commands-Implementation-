#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    unsigned long long lines;
    unsigned long long words;
    unsigned long long bytes;
} Counts;

static int count_stream(FILE *file, Counts *counts) {
    int ch;
    int in_word = 0;

    while ((ch = fgetc(file)) != EOF) {
        counts->bytes++;

        if (ch == '\n') {
            counts->lines++;
        }

        if (isspace((unsigned char)ch)) {
            in_word = 0;
        } else if (!in_word) {
            counts->words++;
            in_word = 1;
        }
    }

    if (ferror(file)) {
        return 1;
    }

    return 0;
}

static void print_counts(const Counts *c, int show_l,
                         int show_w, int show_c, const char *name) {
    if (show_l) printf("%7llu", c->lines);
    if (show_w) printf("%7llu", c->words);
    if (show_c) printf("%7llu", c->bytes);

    if (name != NULL) {
        printf(" %s", name);
    }

    putchar('\n');
}

int main(int argc, char *argv[]) {
    int show_l = 0, show_w = 0, show_c = 0;
    int i = 1;
    int files_start;
    int status = 0;
    int file_count = 0;

    while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
        if (strcmp(argv[i], "--") == 0) {
            i++;
            break;
        }

        for (int j = 1; argv[i][j] != '\0'; j++) {
            if (argv[i][j] == 'l') show_l = 1;
            else if (argv[i][j] == 'w') show_w = 1;
            else if (argv[i][j] == 'c') show_c = 1;
            else {
                fprintf(stderr, "my_wc: unknown option: -%c\n",
                        argv[i][j]);
                return 1;
            }
        }
        i++;
    }

    if (!show_l && !show_w && !show_c) {
        show_l = show_w = show_c = 1;
    }

    files_start = i;

    if (i == argc) {
        Counts c = {0};
        if (count_stream(stdin, &c) != 0) {
            perror("my_wc: stdin");
            return 1;
        }
        print_counts(&c, show_l, show_w, show_c, NULL);
        return 0;
    }

    Counts total = {0};

    for (; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");

        if (file == NULL) {
            perror(argv[i]);
            status = 1;
            continue;
        }

        Counts c = {0};

        if (count_stream(file, &c) != 0) {
            perror(argv[i]);
            status = 1;
        } else {
            print_counts(&c, show_l, show_w, show_c, argv[i]);
            total.lines += c.lines;
            total.words += c.words;
            total.bytes += c.bytes;
            file_count++;
        }

        fclose(file);
    }

    if (file_count > 1) {
        print_counts(&total, show_l, show_w, show_c, "total");
    }

    (void)files_start;
    return status;
}