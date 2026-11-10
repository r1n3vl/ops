#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<ctype.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char**argv)
{
    int fd = open(argv[1], O_RDWR);
    if(fd==-1)
    {
        perror("open");
        return EXIT_FAILURE;
    }
    struct stat sb;
    if(lstat(argv[1], &sb)==-1)
    {
        perror("lstat");
        return EXIT_FAILURE;
    }
    size_t length = sb.st_size;
    char *buf = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(buf == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }
    for(size_t i =0; i < length;i++)
    {
        if(islower(buf[i]))
        {   
            char temp = buf[i];
            if((buf[i]=toupper(temp))==temp)
            {
                perror("toupper");
                return EXIT_FAILURE;
            }
        }
    }
    printf("%s\n", buf);
    msync(buf, length, MS_SYNC);
    munmap(buf, length);
    close(fd);
    return EXIT_SUCCESS;
}