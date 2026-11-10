#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char** argv)
{
    if(argc != 2)
    {
        perror("argc");
        return EXIT_FAILURE;
    }
    char *name = "Yar";
    char *separator=": ";
    char *message ="Hello";

    char first[5]={0};
    char second[8]={0};
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0600);
    if(fd == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    struct iovec buffers[3] = {
        {.iov_base=name, .iov_len=strlen(name)},
        {.iov_base=separator, .iov_len=strlen(separator)},
        {.iov_base=message, .iov_len=strlen(message)}

    };

    ssize_t written = writev(fd, buffers, 3);
    if(written == -1)
    {
        perror("write");
        return EXIT_FAILURE;
    }
    lseek(fd, 0, SEEK_SET);

    struct iovec bufs[2] = {
        {.iov_base=first, .iov_len = sizeof(first)-1},
        {.iov_base=second, .iov_len = sizeof(second)-1}

    };

    ssize_t readen = readv(fd, bufs, 2);
    first[4]='\0';
    second[7]='\0';
    if(readen == -1)
    {
        perror("readv");
        return EXIT_FAILURE;
    }

    char msg[100];
    char msg2[100];
    snprintf(msg, 100, "first: %s\n", first);
    write(STDOUT_FILENO, msg, strlen(msg));
    snprintf(msg2, 100, "second: %s\n", second);
    write(STDOUT_FILENO, msg2, strlen(msg2));


    if(close(fd) == -1)
    {
        perror("close");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;


}