#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *cwd = getcwd(NULL, 0);

    if (cwd == NULL) {
        perror("my_pwd");
        return 1;
    }

    puts(cwd);
    free(cwd);
    return 0;
}