
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t now = time(NULL);

    if (now == (time_t)-1) {
        perror("my_date");
        return 1;
    }

    struct tm *local = localtime(&now);

    if (local == NULL) {
        fprintf(stderr, "my_date: unable to convert local time\n");
        return 1;
    }

    char buffer[128];

    if (strftime(buffer, sizeof(buffer),
                 "%a %b %d %H:%M:%S %Z %Y", local) == 0) {
        fprintf(stderr, "my_date: failed to format date\n");
        return 1;
    }

    puts(buffer);
    return 0;
}
