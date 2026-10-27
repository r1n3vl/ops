#include <asm-generic/errno-base.h>
#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_PATH 256

#define ERR(source)                                                            \
  (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__),             \
   exit(EXIT_FAILURE))

struct meta {
  char *name;
  char *author;
  char *genre;
};

struct filest{
    char name[65];
    unsigned size;
};
// join 2 path. returned pointer is for newly allocated memory and must be freed
char *join_paths(const char *path1, const char *path2) {
  char *res;
  const int l1 = strlen(path1);
  if (path1[l1 - 1] == '/') {
    res = malloc(strlen(path1) + strlen(path2) + 1);
    if (!res)
      ERR("malloc");
    strcpy(res, path1);
  } else {
    res = malloc(strlen(path1) + strlen(path2) + 2); // additional space for "/"
    if (!res)
      ERR("malloc");
    strcpy(res, path1);
    res[l1] = '/';
    res[l1 + 1] = 0;
  }
  return strcat(res, path2);
}

struct meta getatg(const char *path) {
  FILE *fd;
  struct meta mdt;
  mdt.author = "missing!";
  mdt.genre = "missing!";
  mdt.name = "missing!";

  if ((fd = fopen(path, "r")) == NULL) {
    ERR("fopen");
  }
  char *metadata = NULL;
  size_t size = 0;
  ssize_t n;
  char *ok;
  char *tok;
  char *dupl;
  while ((n = getline(&metadata, &size, fd)) != -1) {
    tok = strtok(metadata, ":");
    if (tok == NULL)
      continue;

    if (tok == NULL) {
      printf("The books metadata file is empty stupid faggot\n");
    }

    if (strcmp(tok, "title") == 0) {
      ok = strdup(strtok(NULL, ":"));
      mdt.name = strtok(ok, "\n");
    } else if (strcmp(tok, "author") == 0) {
      ok = strdup(strtok(NULL, ":"));
      mdt.author = strtok(ok, "\n");

    } else if (strcmp(tok, "genre") == 0) {
      ok = strdup(strtok(NULL, ":"));
      mdt.genre = strtok(ok, "\n");
    }
  }
  
  fclose(fd);
  return mdt;
}

int tw(const char *fpath, const struct stat *st, int typeflag,
       struct FTW *ftwbuf) {
  DIR *dirp;
        
  switch (typeflag) {
  case FTW_F: {
    char *kip = realpath("index/by-visible-title", NULL);
    char *cip = realpath("index/by-title", NULL);
    char *lip = realpath("index/by-genre", NULL);
    const char *name = strrchr(fpath, '/');
    if (name == NULL) {
      name = (char *)fpath;
    } else {
      name++;
    }
    struct meta ad = getatg(fpath);
    
    if (strlen(ad.name) > 64) {
      ad.name[64] = '\0';
    }
    // printf("%s", kip);
    char *linkedpaths = join_paths(kip, name);
    // if((symlink(fpath, linkedpaths))==-1)
    // {
    //     ERR("symlink");
    // }
    free(linkedpaths);
    linkedpaths = join_paths(cip, ad.name);
    // if ((symlink(fpath, linkedpaths)) == -1) {
    //   ERR("symlink");
    // }
    // free(linkedpaths);
    // linkedpaths = join_paths(lip, ad.genre);
    // DIR *dir = opendir(linkedpaths);
    // if (dir) {
    //   /* Directory exists. */

    //   if ((symlink(fpath, join_paths(linkedpaths, ad.name))) == -1) {
    //     ERR("symlink");
    //   }
    // } else if (ENOENT == errno) {
    //   /* Directory does not exist. */
    //   mkdir(linkedpaths, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    //   if ((symlink(fpath, join_paths(linkedpaths, ad.name))) == -1) {
    //     ERR("symlink");
    //   }
    // } else {
    //   /* opendir() failed for some other reason. */
    //   ERR("opendir");
    // }
    free(linkedpaths);
    free(cip);
    free(kip);
    free(lip);
    break;
  }
  default: {
    break;
  }
  }
  return 0;
}

void usage(int argc, char **argv) {
  (void)argc;
  fprintf(stderr, "USAGE: %s path\n", argv[0]);
  exit(EXIT_FAILURE);
}

void chilloviy(struct filest piz){
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    bool found = false;
    if((dirp = opendir("/home/user/opsfinal/lab1_4/index/by-title"))==NULL)
    {
        ERR("opendir");
    }
    do{
       
        errno = 0;
        if((dp=readdir(dirp))!=NULL)
        {
            if(fstatat(dirfd(dirp),dp->d_name, &filestat, 0))
            {
                ERR("filestat");
            }
            if(S_ISREG(filestat.st_mode))
            {
                if(strcmp(dp->d_name,piz.name)==0){
                    if(filestat.st_size != piz.size)
                    {
                     printf("Book \"%s\" size mismatch (%u vs %zd)\n", dp->d_name, piz.size, filestat.st_size);
                    }
                    found = true;
                }

            }
        }
    }while(dp != NULL);
    if(!found) {printf("Book \"%s\" is missing\n", piz.name);}
    if (closedir(dirp) == -1)
    {ERR("closedir");}
}

int main(int argc, char **argv) {
    
    if (nftw("/home/user/opsfinal/lab1_4/library", tw, MAX_PATH, FTW_PHYS) ==
      -1) {
    ERR("nftw");
  }
    int fd = open("database_correct", O_RDONLY);
    if (fd == -1)
    {ERR("open");}
    struct filest fil;
   
    
    struct iovec iov[2] = {
        {&fil.size, sizeof(fil.size)},
        {fil.name, 64}
        
    };
    int iovcnt = 2;
  //    struct meta dmk = getatg(argv[1]);

  //    printf("author: %s\ntitle: %s\ngenre: %s\n",dmk.author, dmk.name,
  //    dmk.genre);
  ssize_t bytes;
    while((bytes = readv(fd, iov, iovcnt))==68)
    {
        fil.name[64]='\0';
        //  printf("Size: %u\n", fil.size);
        //  printf("Name: %s\n", fil.name);
         chilloviy(fil);
        
    }
     if (bytes == -1)
           { ERR("readv");}


  if (close(fd) == -1)
   { ERR("close");
}
  
}
