
// #define _POSIX_C_SOURCE 200809L

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <errno.h>

// #define MAX_INPUT 4096
// #define MAX_ARGS 128
// #define MAX_PATH 4096

// static const char *allowed_commands[] = {
//     "echo", "my_echo",
//     "cat", "my_cat",
//     "wc", "my_wc",
//     "cp", "my_cp",
//     "ls", "my_ls",
//     "chmod", "my_chmod",
//     "pwd", "my_pwd",
//     "date", "my_date",
//     "grep", "my_grep",
//     NULL
// };

// static const char *resolve_command(const char *command)
// {
//     for (int i = 0; allowed_commands[i] != NULL; i += 2) {
//         if (strcmp(command, allowed_commands[i]) == 0) {
//             return allowed_commands[i + 1];
//         }

//         if (allowed_commands[i + 1] != NULL &&
//             strcmp(command, allowed_commands[i + 1]) == 0) {
//             return allowed_commands[i + 1];
//         }
//     }

//     return NULL;
// }

// static int parse_command(char *input, char *args[])
// {
//     int argc = 0;
//     char *saveptr = NULL;
//     char *token = strtok_r(input, " \t\r\n", &saveptr);

//     while (token != NULL && argc < MAX_ARGS - 1) {
//         args[argc++] = token;
//         token = strtok_r(NULL, " \t\r\n", &saveptr);
//     }

//     args[argc] = NULL;
//     return argc;
// }

// static int run_builtin(char *args[])
// {
//     if (strcmp(args[0], "exit") == 0) {
//         exit(EXIT_SUCCESS);
//     }

//     if (strcmp(args[0], "cd") == 0) {
//         if (args[1] != NULL && args[2] != NULL) {
//             fprintf(stderr, "myshell: cd: too many arguments\n");
//             return 1;
//         }

//         const char *path = args[1];

//         if (path == NULL) {
//             path = getenv("HOME");
//         }

//         if (path == NULL) {
//             fprintf(stderr, "myshell: cd: HOME is not set\n");
//         } else if (chdir(path) != 0) {
//             perror("myshell: cd");
//         }

//         return 1;
//     }

//     return 0;
// }

// static void execute_command(char *args[])
// {
//     const char *executable = resolve_command(args[0]);

//     if (executable == NULL) {
//         fprintf(stderr, "myshell: unsupported command: %s\n", args[0]);
//         fprintf(stderr, "Allowed: echo cat wc cp ls chmod pwd date grep\n");
//         return;
//     }

//     char path[MAX_PATH];

//     int written = snprintf(path, sizeof(path), "./bin/%s", executable);

//     if (written < 0 || (size_t)written >= sizeof(path)) {
//         fprintf(stderr, "myshell: executable path too long\n");
//         return;
//     }

//     if (access(path, X_OK) != 0) {
//         fprintf(stderr, "myshell: executable unavailable: %s\n", path);
//         return;
//     }

//     char *exec_args[MAX_ARGS];
//     exec_args[0] = path;

//     int i = 1;
//     for (int j = 1; args[j] != NULL && i < MAX_ARGS - 1; j++) {
//         exec_args[i++] = args[j];
//     }
//     exec_args[i] = NULL;

//     pid_t pid = fork();

//     if (pid < 0) {
//         perror("myshell: fork");
//         return;
//     }

//     if (pid == 0) {
//         execv(path, exec_args);
//         perror(args[0]);
//         _exit(127);
//     }

//     int status;
//     pid_t result;

//     do {
//         result = waitpid(pid, &status, 0);
//     } while (result < 0 && errno == EINTR);

//     if (result < 0) {
//         perror("myshell: waitpid");
//     }
// }

// int main(void)
// {
//     char input[MAX_INPUT];
//     char *args[MAX_ARGS];

//     while (1) {
//         printf("myshell> ");
//         fflush(stdout);

//         if (fgets(input, sizeof(input), stdin) == NULL) {
//             if (feof(stdin)) {
//                 putchar('\n');
//                 break;
//             }

//             if (errno == EINTR) {
//                 clearerr(stdin);
//                 continue;
//             }

//             perror("myshell: input");
//             break;
//         }

//         if (strchr(input, '\n') == NULL && !feof(stdin)) {
//             int ch;
//             while ((ch = getchar()) != '\n' && ch != EOF) {
//                 /* Discard excess input. */
//             }

//             fprintf(stderr, "myshell: command too long\n");
//             continue;
//         }

//         int argc = parse_command(input, args);

//         if (argc == 0) {
//             continue;
//         }

//         if (run_builtin(args)) {
//             continue;
//         }

//         execute_command(args);
//     }

//     return EXIT_SUCCESS;
// }



#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_INPUT 4096
#define MAX_ARGS 128
#define MAX_PATH 4096

static const char *allowed_commands[][2] = {
    {"echo",  "my_echo"},
    {"cat",   "my_cat"},
    {"wc",    "my_wc"},
    {"cp",    "my_cp"},
    {"ls",    "my_ls"},
    {"chmod", "my_chmod"},
    {"pwd",   "my_pwd"},
    {"date",  "my_date"},
    {"grep",  "my_grep"},
    {NULL, NULL}
};

/* Map permitted command names to our custom executables. */
static const char *resolve_command(const char *command)
{
    for (int i = 0; allowed_commands[i][0] != NULL; i++) {
        if (strcmp(command, allowed_commands[i][0]) == 0 ||
            strcmp(command, allowed_commands[i][1]) == 0) {
            return allowed_commands[i][1];
        }
    }

    return NULL;
}

/*
 * Parse arguments while removing matching single/double quotes.
 * Quoted spaces stay inside one argument.
 */
static int parse_command(char *input, char *args[])
{
    char *src = input;
    char *dst = input;
    int argc = 0;

    while (*src != '\0') {
        while (*src == ' ' || *src == '\t' ||
               *src == '\r' || *src == '\n') {
            src++;
        }

        if (*src == '\0') {
            break;
        }

        if (argc >= MAX_ARGS - 1) {
            fprintf(stderr, "myshell: too many arguments\n");
            return -1;
        }

        args[argc++] = dst;
        char quote = '\0';

        while (*src != '\0') {
            if (quote != '\0') {
                if (*src == quote) {
                    quote = '\0';
                    src++;
                } else {
                    *dst++ = *src++;
                }
            } else if (*src == '"' || *src == '\'') {
                quote = *src++;
            } else if (*src == ' ' || *src == '\t' ||
                       *src == '\r' || *src == '\n') {
                src++;
                break;
            } else {
                *dst++ = *src++;
            }
        }

        if (quote != '\0') {
            fprintf(stderr, "myshell: unmatched quote\n");
            return -1;
        }

        *dst++ = '\0';
    }

    args[argc] = NULL;
    return argc;
}

/* Built-in commands must run inside the shell process. */
static int run_builtin(char *args[])
{
    if (strcmp(args[0], "exit") == 0) {
        if (args[1] != NULL) {
            fprintf(stderr, "myshell: exit takes no arguments\n");
            return 1;
        }

        exit(EXIT_SUCCESS);
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL && args[2] != NULL) {
            fprintf(stderr, "myshell: cd: too many arguments\n");
            return 1;
        }

        const char *path = args[1];

        if (path == NULL) {
            path = getenv("HOME");
        }

        if (path == NULL) {
            fprintf(stderr, "myshell: cd: HOME is not set\n");
        } else if (chdir(path) != 0) {
            perror("myshell: cd");
        }

        return 1;
    }

    return 0;
}

/* Execute an allowed command, optionally redirecting standard output. */
static void execute_command(char *args[])
{
    const char *executable = resolve_command(args[0]);

    if (executable == NULL) {
        fprintf(stderr, "myshell: unsupported command: %s\n", args[0]);
        fprintf(stderr,
                "Allowed commands: echo, cat, wc, cp, ls, chmod, "
                "pwd, date, grep, cd, exit\n");
        return;
    }

    char *command_args[MAX_ARGS];
    char *output_file = NULL;
    int append = 0;
    int argc = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0 ||
            strcmp(args[i], ">>") == 0) {

            if (output_file != NULL || args[i + 1] == NULL) {
                fprintf(stderr,
                        "myshell: invalid output redirection\n");
                return;
            }

            append = (strcmp(args[i], ">>") == 0);
            output_file = args[++i];

            if (args[i + 1] != NULL) {
                fprintf(stderr,
                        "myshell: unexpected argument after filename\n");
                return;
            }

            break;
        }

        if (argc >= MAX_ARGS - 1) {
            fprintf(stderr, "myshell: too many arguments\n");
            return;
        }

        command_args[argc++] = args[i];
    }

    command_args[argc] = NULL;

    if (argc == 0) {
        fprintf(stderr, "myshell: missing command\n");
        return;
    }

    char path[MAX_PATH];

    int written = snprintf(path, sizeof(path), "./bin/%s", executable);

    if (written < 0 || (size_t)written >= sizeof(path)) {
        fprintf(stderr, "myshell: executable path too long\n");
        return;
    }

    if (access(path, X_OK) != 0) {
        perror(path);
        return;
    }

    command_args[0] = path;

    pid_t pid = fork();

    if (pid < 0) {
        perror("myshell: fork");
        return;
    }

    if (pid == 0) {
        if (output_file != NULL) {
            int flags = O_WRONLY | O_CREAT;

            flags |= append ? O_APPEND : O_TRUNC;

            int fd = open(output_file, flags, 0666);

            if (fd < 0) {
                perror(output_file);
                _exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("myshell: dup2");
                close(fd);
                _exit(1);
            }

            close(fd);
        }

        execv(path, command_args);

        perror(args[0]);
        _exit(127);
    }

    int status;
    pid_t result;

    do {
        result = waitpid(pid, &status, 0);
    } while (result < 0 && errno == EINTR);

    if (result < 0) {
        perror("myshell: waitpid");
    }
}

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (feof(stdin)) {
                putchar('\n');
                break;
            }

            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }

            perror("myshell: input");
            break;
        }

        if (strchr(input, '\n') == NULL && !feof(stdin)) {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard the remainder of an oversized command. */
            }

            fprintf(stderr, "myshell: command too long\n");
            continue;
        }

        int argc = parse_command(input, args);

        if (argc <= 0) {
            continue;
        }

        if (run_builtin(args)) {
            continue;
        }

        execute_command(args);
    }

    return EXIT_SUCCESS;
}
