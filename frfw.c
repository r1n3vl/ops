#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char**argv)
{
    int num[5]={1, 2, 3, 4 ,5};
    int new_num[5];
    size_t read;
    FILE *fd= fopen("binary.bin", "wb");
    if(fd == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }
    read = fwrite(num, sizeof(int), 5, fd);
    if(read != 5)
    {
        perror("fwrite");
    }
    fclose(fd);
    fd = fopen("binary.bin", "rb");
    size_t nread= fread(new_num, sizeof(int), read, fd);
    if(nread != read)
    {
        perror("read");
        return EXIT_FAILURE;
    }
    for(int i =0; i < nread; i++)
   { printf("new_num: %d", new_num[i]);}
   fclose(fd);
   return EXIT_SUCCESS;
}   