
#include <stdbool.h>
#include <stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <errno.h>
#include<unistd.h>
#include <dirent.h>
#include <string.h>

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#define MAX_PATH 101
 char **paths = NULL;
 int count = 0;
//  char* envpath = getenv("L1_OUTPUFILE");
 const char *get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    
    // Ensure the dot exists, isn't the first character (hidden file like .bashrc),
    // and isn't the final character (e.g., "filename.")
    if (!dot || dot == filename || *(dot + 1) == '\0') {
        return ""; // No valid extension found
    }
    return dot + 1; // Returns the extension (e.g., "txt")
}
void usage(char *pname)
{
    fprintf(stderr, "USAGE:%s -n Name -p OCTAL -s SIZE\n", pname);
    exit(EXIT_FAILURE);
}
void scan_dir(char* optar, char* envpath, bool burmaldok, FILE *f )
{
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
   
    // ftruncate(f,0);
    if((dirp = opendir("."))==NULL){
        ERR("opendir");}
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            
           
            if(optar != NULL ){
                if (lstat(dp->d_name, &filestat))
                {ERR("lstat");
                }
                
                else if (S_ISREG(filestat.st_mode) ){
                    const char* voss = get_file_extension(dp->d_name);  
                    if(strcmp(optar, voss) == 0)
                {
                    if(burmaldok){
                    fprintf(f,"Filename:%s\n", dp->d_name);
                    }
                    else {
                    printf("Filename:%s\n", dp->d_name);
                    }
                }
                }
        
            }
            else
            {
                if(lstat(dp->d_name, &filestat))
                {
                    ERR("LSTAT");
                }
                else if(S_ISREG(filestat.st_mode)){
           
                    if(burmaldok){
                    fprintf(f,"Filename:%s\n", dp->d_name);
                    }
                    else {
                    printf("Filename:%s\n", dp->d_name);
                    }
                }
               
            }
            
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
}
char* scan_dirfd()
{
    DIR *dirp;
    struct dirent *dp;
    struct stat filestat;
    char *path;
    if((dirp = opendir("."))==NULL){
        ERR("opendir");}
    do
    {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if (lstat(dp->d_name, &filestat))
                ERR("lstat");
            
            else if(S_ISDIR(filestat.st_mode))
            {
                if(strcmp(dp->d_name,".") !=0 && strcmp(dp->d_name,"..")!=0)
                    {
                        path = realpath(dp->d_name, NULL);
                   
                char **temp = realloc(paths,(count+1)*sizeof(*paths));
                paths = temp;
                 paths[count++] = strdup(path);

                    }
                
            }
            
            
        }
    } while (dp != NULL);

    if (errno != 0)
        ERR("readdir");
    if (closedir(dirp))
        ERR("closedir");
    return path;
}
int main(int argc, char **argv)
{
   printf("PIZDA\n");
    
    int c;
     char* envpath = getenv("L1_OUTPUFILE");
    if(envpath==NULL)
     {
            ERR("Envpath");
     }
     
    
    int dir_count = 0;
    int depth = 1;
    char path[MAX_PATH];
   char **root_paths = NULL;
    char *txt=NULL;
    char *primary_path;
    bool burmalda = false;
    if(getcwd(path, MAX_PATH)==NULL)
    {
        ERR("getcwd");
    }
       
    printf("---labtask2 first part (lists all the files of provided directories)---\n");
    
    while ((c = getopt(argc, argv, "p:d:e:o")) != -1){
        switch (c)
        {
            case 'p':
               { char **temp = realloc(root_paths, (dir_count + 1) * sizeof(*root_paths));

                if (temp == NULL)
                    {ERR("realloc");}

                root_paths = temp;
                root_paths[dir_count] = optarg;
                dir_count++;}
            break;
            case 'd':
                {depth = strtol(optarg, (char **)NULL, 10);
                break;}
            case 'e':
            {
                txt = optarg;
                break;
            }
            case 'o':
            {
                burmalda = true;
                break;
            }
            case '?':
           
            default:
                usage(argv[0]);
        }
    }   
    FILE *f = stdout;
    if(burmalda){
     
    // ftruncate(f,0);
    f = fopen(envpath,"w");
    }
  
    for(int a = 0; a < dir_count; a++){
            if(chdir(root_paths[a])){
            ERR("chdir");
            }
            if(burmalda){
            fprintf(f,"%s:\n", root_paths[a]);
            }
            else {
            printf("%s:\n", root_paths[a]);
            
            }
            if(depth == 1)
            {
                // printf("%s",path);
                scan_dir(txt, envpath, burmalda,f);
                if(burmalda){
                fprintf(f,"Depth: %d\n", depth);
                }
                else {
                printf("Depth: %d\n", depth);
                
                }
                return EXIT_SUCCESS;
            }
            //  printf("BURMALDA");
            scan_dir(txt, envpath,burmalda,f);
           primary_path = scan_dirfd();
           
            int pizda = 1;
            while(depth > pizda){
            //  printf("\n");
            
            if(paths != NULL)
            {
                
                for(int j = 0; j < count; j++)
                {
                    if(burmalda)
                    {
                    fprintf(f,"%s:\n", paths[j]);
                    }
                    else {
                    printf("%s:\n", paths[j]);
                    
                    }
                    if(chdir(paths[j]))
                    {
                        ERR("pizods");
                    }
                    scan_dir(txt, envpath,burmalda,f);
                    //printf("\n");
                    
                   
                  
                     
                }
                free(paths);
                paths=NULL;
                count = 0;

            }
            else {
            return EXIT_SUCCESS;
            }
        
        // if (chdir(path))
        //      ERR("chdir");

        pizda++;
        
        scan_dirfd();
        
        }
        if(burmalda){  
        fprintf(f,"Depth: %d\n", depth);
        }
        else {

        printf("Depth: %d\n", depth);
        
        }
    
    }
    fclose(f);
    return EXIT_SUCCESS;
}