#include <asm-generic/errno-base.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
ssize_t bulk_write(int fd, const char* buffer, size_t nbytes)
{
    size_t total = 0;
    while(nbytes > total)
    {
        size_t count = write(fd, buffer + total, (size_t)(nbytes-total));
        if(count == (size_t)-1)
        {
            if(errno ==EINTR)
            {
                continue;
            }
            ERR("write");
        }
        total += count;

    }
    return total;
}
int main(int argc, char **argv)
{
    if(argc!=2)
    {
        printf("USAGE: ./task1 filename");
        return EXIT_FAILURE;
    }
    char buffer[64];
    int fd = open(argv[1], O_RDONLY);
    ssize_t count = 0; 
    if(fd == -1)
    {
        ERR("open");
    }
    for(;;){
    ssize_t bytes =  read(fd, buffer, sizeof(buffer));
    if(bytes == -1)
    {
        ERR("read");
    }
    if(bytes == 0)
    {
        break;
    }
    ssize_t writen = bulk_write(STDOUT_FILENO,buffer, bytes );
    if(writen != bytes)
    {
        ERR("write");
    }   
    count += bytes;
    }

    char msg[100];
    

    int len = snprintf(msg, sizeof(msg), "Total bytes: %ld\n", (long)count);
    ssize_t g = write(STDERR_FILENO, msg, len);
    if(g != len)
    {
        ERR("write");
    }

    if(close(fd) == -1){
        ERR("close");
    }
    return EXIT_SUCCESS;
}