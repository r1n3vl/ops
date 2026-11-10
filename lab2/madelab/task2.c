#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include <errno.h>
#include <sys/stat.h>

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
// ssize_t bulk_read(int fd, char *buf, ssize_t nbytes)
// {
//     ssize_t total = 0;
//     while(total < nbytes)
//     {
//         ssize_t count = read(fd, buf+total, (size_t)(nbytes-total));
//         if(count == -1)
//         {
//             if(errno == EINTR)
//             {
//                 continue;
//             }
//             ERR("read");
//         }
//         if(count == 0)
//         {
//             ERR("read");
//         }
//         total += count;
//     }
//     return total;
// }
int main(int argc, char **argv)
{
     int kaban = 0;
    if(argc == 5)
    {
        if(strcmp ("-r", argv[4])==0)
        {
            kaban = 1;
        }
    }
    if(argc != 5 && argc != 4)
    {
        ERR("argc");
    }
    int fd;
    int fd1;
   
    fd1 = open(argv[2], O_RDONLY);
    

    if(fd1 == -1)
    {
        ERR("open");
    }
    if(strcmp("-n", argv[1])==0)
    {
        if(access(argv[3], F_OK)==0)
        {
            ERR("FILE exists");
        }
        fd = open(argv[3],O_WRONLY | O_CREAT, 0644);
        if(fd == -1)
        {
            ERR("open");
        }
    }
    else if(strcmp("-a", argv[1])==0)
    {
        fd = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, 0644);
        if(fd == -1)
        {
            ERR("open");
        }
    }
    else if(strcmp("-o", argv[1])==0)
    {
        fd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if(fd == -1)
        {
            ERR("open");
        }
    }
    else {
    ERR("args");
    }

    
    
    struct stat sb;
    char *rp = realpath(argv[2], NULL);
    if(lstat(rp, &sb)==-1)
    {
        ERR("lstat");
    }
    
    char buffer[sb.st_size];

    
    if(kaban)
    {
        const ssize_t len = read(fd1, buffer, sb.st_size);
        if(len == -1)
        {
           
            ERR("read");
        }
        if(buffer[len-1]=='\n')
        {
        printf("%s",buffer);
        for(int i = 0; i < (len-1)/2; i++)
        {
            char temp = buffer[len-2-i];
            buffer[len-2-i]=buffer[i];
            buffer[i]=temp;
        }
        }
        else {
            for(int i = 0; i < (len-1)/2; i++)
        {
            char temp = buffer[len-1-i];
            buffer[len-1-i]=buffer[i];
            buffer[i]=temp;
        }
        }
         printf("%s", buffer);
        const ssize_t writ = bulk_write(fd, buffer, len);
        if(writ == -1)
        {
            ERR("write");
        }

        char bufd[100];
        snprintf(bufd, sizeof(bufd), "Bytes written: %ld\n", writ);
        write(STDERR_FILENO, bufd, sizeof(bufd));
    }
    else {
        printf("HELLO\n");
       const ssize_t len = read(fd1, buffer, sb.st_size);
        if(len == -1)
        {
           
            ERR("read");
        }
         printf("%s", buffer);
        const ssize_t writ = bulk_write(fd, buffer, sb.st_size);
        if(writ == -1)
        {
            ERR("write");
        }
        char bufd[100];
        snprintf(bufd, sizeof(bufd), "Bytes written: %ld\n", writ);
        write(STDERR_FILENO, bufd, sizeof(bufd));

    }
    
    
    if(close(fd)==-1)
    {
        ERR("close");
    }
     if(close(fd1)==-1)
    {
        ERR("close");
    }
    free(rp);
    

    return EXIT_SUCCESS;
}