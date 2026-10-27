#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#define MAX_PATH 256
int total_files = 0;
int passed_size_filter = 0;
long depth;
int callback(const char *fpath,
             const struct stat *sb,
             int typeflag,
             struct FTW *ftwbuf)
{
    // TODO:
    // 1. Check whether the entry is a regular file
    // 2. Extract the filename
    // 3. Print filename, size, and depth
    // 4. Increment total_files
   const char *filename;
   const char *dirname;
//    printf("%d\n", depth);
   if(typeflag == FTW_D && ftwbuf->level >= depth)
   {
    return FTW_SKIP_SUBTREE;
   }
   if(typeflag == FTW_D){
        dirname = fpath + ftwbuf->base;
        if(strcmp(dirname,"backup")==0)
        {
            return FTW_SKIP_SUBTREE;
        }
    }
    if(typeflag == FTW_F && S_ISREG(sb->st_mode))
    {
        
        filename = fpath + ftwbuf->base;
       
        total_files++;
        if(sb->st_size > 100)
        {
            passed_size_filter++;
            printf("filename: %s, size: %lld, level: %d\n", filename, (long long)sb->st_size, ftwbuf->level);
        }
    }


    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s DIRECTORY\n", argv[0]);
        return EXIT_FAILURE;
    }
    depth = strtol(argv[2],(char**)NULL, 10);
    
    if (nftw(argv[1], callback, 20, FTW_PHYS | FTW_ACTIONRETVAL ) == -1)
    {
        perror("nftw");
        return EXIT_FAILURE;
    }

    printf("\nTotal regular files: %d\n", total_files);
    printf("\nTotal passed files: %d\n", passed_size_filter);


    return EXIT_SUCCESS;
}