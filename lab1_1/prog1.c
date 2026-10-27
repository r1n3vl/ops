#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#define MAX_PATH 101
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

ssize_t scandirforsize(char* path)
{
    DIR* dirp;
    struct dirent *dp;
    struct stat filestat;
    ssize_t butes = 0;
    if((dirp = opendir(path))==NULL)
    {
        printf("No access to folder %s\n", path);
        return butes;
    }
    
    do{
        errno = 0;
        if((dp = readdir(dirp))!=NULL)
        {
           if(fstatat(dirfd(dirp),dp->d_name, &filestat, AT_SYMLINK_NOFOLLOW))
        {
            ERR("fstatat");
        }
        butes += filestat.st_size;
        }
        
        
    }while(dp!=NULL);
     if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
    return butes;
}

int main(int argc, char** argv)
{
char path[MAX_PATH];
 FILE *fd;
 fd = fopen("output.txt", "w");
 ssize_t filesize = 0;
 ssize_t intendedsize;
for (int i = 1; i + 1 < argc; i += 2)
{
    intendedsize = strtol(argv[i + 1], NULL, 10);
   
    if((filesize = scandirforsize(argv[i]))==0)
    {
        continue;
    }

    if (filesize > intendedsize)
    {
        fprintf(fd, "%s\n", argv[i]);
    }
}
// truncate("output.txt", SEEK_END);
if(fclose(fd)!=0)
{
    ERR("PIZDA");
}
return EXIT_SUCCESS;
}