#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static int contains_text(const char *line, const char *pattern, int ignore_case)
{
    size_t pattern_len = strlen(pattern);

    if (pattern_len == 0) {
        return 1;
    }

    for (const char *p = line; *p != '\0'; p++) {
        if (ignore_case) {
            if (strncasecmp(p, pattern, pattern_len) == 0) {
                return 1;
            }
        } else if (strncmp(p, pattern, pattern_len) == 0) {
            return 1;
        }
    }

    return 0;
}

static int search_file(const char *filename, const char *pattern,
                       int show_line_numbers, int ignore_case,
                       int show_filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror(filename);
        return 2;
    }

    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;
    unsigned long line_number = 0;
    int found = 0;
    int error = 0;

    while ((length = getline(&line, &capacity, file)) != -1) {
        (void)length;
        line_number++;

        if (contains_text(line, pattern, ignore_case)) {
            if (show_filename) {
                printf("%s:", filename);
            }

            if (show_line_numbers) {
                printf("%lu:", line_number);
            }

            fputs(line, stdout);

            if (line[0] != '\0' && line[strlen(line) - 1] != '\n') {
                putchar('\n');
            }

            found = 1;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "my_grep: error reading %s\n", filename);
        error = 2;
    }

    free(line);

    if (fclose(file) != 0) {
        perror(filename);
        error = 2;
    }

    if (error) {
        return error;
    }

    return found ? 0 : 1;
}

int main(int argc, char *argv[])
{
    int show_line_numbers = 0;
    int ignore_case = 0;
    int index = 1;

    while (index < argc && argv[index][0] == '-' &&
           argv[index][1] != '\0') {
        if (strcmp(argv[index], "--") == 0) {
            index++;
            break;
        }

        for (size_t j = 1; argv[index][j] != '\0'; j++) {
            if (argv[index][j] == 'n') {
                show_line_numbers = 1;
            } else if (argv[index][j] == 'i') {
                ignore_case = 1;
            } else {
                fprintf(stderr, "my_grep: unsupported option: -%c\n",
                        argv[index][j]);
                return 2;
            }
        }

        index++;
    }

    if (argc - index < 2) {
        fprintf(stderr, "Usage: my_grep [-i] [-n] PATTERN FILE...\n");
        return 2;
    }

    const char *pattern = argv[index++];
    int file_count = argc - index;
    int matched = 0;
    int had_error = 0;

    for (; index < argc; index++) {
        int result = search_file(argv[index], pattern,
                                 show_line_numbers, ignore_case,
                                 file_count > 1);

        if (result == 0) {
            matched = 1;
        } else if (result == 2) {
            had_error = 1;
        }
    }

    if (had_error) {
        return 2;
    }

    return matched ? 0 : 1;
}