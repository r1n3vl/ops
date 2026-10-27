// #include <bits/getopt_core.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define MAX_PATH 256
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
void usage(char* pname)
{
    fprintf(stderr, "USAGE:%s -n Name -p OCTAL -s SIZE\n", pname);
    exit(EXIT_FAILURE);
}
void scan_dir(char* name)
{
    DIR* dirp;
    struct dirent* dp;
    struct stat filestat;
    char* kabanchik;
    
    if ((dirp = opendir(".")) == NULL)
        ERR("opendir");
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if (lstat(dp->d_name, &filestat))
                ERR("lstat");
            if (S_ISREG(filestat.st_mode))
            {
                if ((kabanchik = strstr(dp->d_name, name)) != NULL)
                {
                    printf("%s\n", kabanchik);
                    unlink(kabanchik);
                    remove(kabanchik);
                }
            }
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
}

void removeline(char* line, FILE* f)
{
    // FILE *fd;
    // fd = fopen("dummy", "w+");
    ssize_t sz = 0;
    ssize_t moved = 0;
    char* token;
    errno = -1;
    char bigboss[MAX_PATH];
    char kk[MAX_PATH];
    char* needtoremove;
    char *token2;
    ssize_t mov = 0;
    if (fread(bigboss, sizeof(bigboss), MAX_PATH, f) == -1)
    {
        ERR("fimoz");
    }
    printf("%s\n", bigboss);
    if((token = strtok(bigboss, "\n"))==NULL)
    {
        ERR("EMPTY FILE");
        
    }
    // moved += strlen(token)+1;
    while (token != NULL)
    {
       
        if (token != NULL)
        {
             moved += strlen(token)+1;
            if ((needtoremove = strstr(token, line)) != NULL)
            {   
                // moved+=1;
                printf("%zd", moved);
                printf("NEEDTOREMOVE%s\n", needtoremove);
                mov = moved -(strlen(needtoremove)+1);
                printf("MOVED::%zd\n", mov);
                fseek(f, moved, SEEK_SET);
                // truncate("requierments", mov);
                // printf("FILESIZE::%zd\n", ftell(f));
                // printf("%zd", mov);
                fread(kk,sizeof(kk), MAX_PATH, f);
                // printf("FILEEE%s", kk);
                if((token2 = strtok(kk,"\0" ))!=NULL){
                fseek(f, 0, SEEK_SET);
                // sz = ftell(f);
                truncate("requierments", mov);
               
                fseek(f, 0, SEEK_END);
                //  printf("FILE::%zd, %s\n", sz, token2);
                fwrite(token2, strlen(token2), 1, f);
                }
                else {
                truncate("requierments", mov);
                }
                // printf("NIGG\n");
                
                // printf("%zd", moved);
                // 
            }
            
        }
         token = strtok(NULL, "\n");
        
        //  printf("%s", token);
    }
}
int main(int argc, char** argv)
{
    FILE* fd;
    char originpath[MAX_PATH];
    char* filename = "requierments";
    int c;
    char* token;
    bool entrvevn = false;
    char name[MAX_PATH];
    char version[MAX_PATH];
    char* real;
    
    while ((c = getopt(argc, argv, ":cvi:r:")) != -1)
    {
        switch (c)
        {
            case 'c':
            {
                mkdir(argv[2], 0777);
                getcwd(originpath, MAX_PATH);
                chdir(argv[2]);

                if ((fd = fopen(filename, "a+")) == NULL)
                {
                    ERR("fopen");
                }

                chmod(filename, 0644);
                chdir(originpath);
                break;
            }
            case 'v':
            {
                // printf("%s", optarg);
                DIR *dir = opendir(argv[2]);
                if(dir)
                {
                entrvevn = true;

                }
                else if(ENOENT == errno)
                {
                    ERR("the enviroment doesnt exit");
                }
                else {
                
                    ERR("dir");                
                }
                // printf("fucking shit");
                closedir(dir);
                break;
            }
            case 'i':
            {
                if (entrvevn)
                {
                    // FILE *f;
                    chdir(argv[2]);
                    fd = fopen(filename, "a+");
                    token = strtok(argv[4], "==");
                    strcpy(name, token);
                    strcat(name, " ");
                    token = strtok(NULL, "==");
                    strcpy(version, token);
                    strcat(name, version);
                    printf("%s\n", name);
                    fprintf(fd, "%s\n", name);
                    fclose(fd);
                    fd = fopen(name, "w+");
                    for (int i = 0; i < (MAX_PATH * 10) / 100; i++)
                    {
                        if (fseek(fd, 0, SEEK_END))
                            ERR("fseek");
                        fprintf(fd, "%c", 'A' + (i % ('Z' - 'A' + 1)));
                    }
                    chmod(name, 0444);
                    fclose(fd);
                }

                break;
            }
            case 'r':
            {
                real = realpath(argv[2], NULL);

                printf("%s\n", real);
                chdir(real);
                fd = fopen(filename, "r+");
                removeline(argv[4], fd);
                scan_dir(argv[4]);
                free(real);
                break;
            }
            case '?':
            default:
                usage(argv[0]);
        }
    }

    return EXIT_SUCCESS;
}
