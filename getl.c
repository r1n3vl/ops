#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
int main(void)
{
    char *line = NULL;
    size_t capacity = 0;

    ssize_t nread = getline(&line, &capacity, stdin);

    if (nread != -1)
    {
        printf("Line: %s", line);
        printf("Characters read: %zd\n", nread);
        printf("Buffer capacity: %zu\n", capacity);
    }

    free(line);

    return 0;
}