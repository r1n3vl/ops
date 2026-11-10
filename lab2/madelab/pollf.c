#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
int main(){
    int fd = open("tst.txt", O_RDONLY);

    struct pollfd fds[]=
    {
        {.fd=STDIN_FILENO, .events = POLLIN },
        {.fd= fd, .events = POLLIN}
        
    };
    // struct termios original, modified;
    // if(tcgetattr(STDIN_FILENO, &original)==-1)
    // {
    //     perror("termios");
    //     return EXIT_FAILURE;
    // }
    // modified = original;
    // modified.c_lflag=~ICANON;
    // modified.c_cc[VMIN]=1;
    // modified.c_cc[VTIME]=0;
    // if (tcsetattr(STDIN_FILENO, TCSANOW, &modified) == -1)
    // {
    // perror("tcsetattr");
    // return EXIT_FAILURE;
    // }

    char boss[256];
    for(;;)
    {
        int ready = poll(fds, 2, 3000);
        if(ready == -1)
        {
            if(errno==EINTR)
            {
                continue;
            }

            perror("poll");
            return EXIT_FAILURE;
        }
        if(ready==0)
        {
            char msg[100];
            if(snprintf(msg, 100, "Stil waiting...\n")<0)
            {
                perror("SNPRINTF");
                return EXIT_FAILURE;
            }
            if(write(STDERR_FILENO, msg, strlen(msg))==-1)
            {
                perror("write");
                return EXIT_FAILURE;
            }
            continue;
        }
        for(int i = 0; i < 2; i++){
        if(fds[i].revents & (POLLIN| POLLHUP)){
        ssize_t readen = read(fds[i].fd, boss, sizeof(boss));
        if(readen == -1)
        {
            perror("read");
            return EXIT_FAILURE;
        }

        if(readen == 0 )
        {

            if (fds[i].fd == STDIN_FILENO)
            {
                close(fd);
                return EXIT_SUCCESS;
            }
            close(fds[i].fd);
            fds[i].fd=-1;
            continue;
        }
        write(STDOUT_FILENO, "Recieved: ", sizeof("Recieved: "));

        write(STDOUT_FILENO, boss, readen);
    }
    }
         
        // write(STDOUT_FILENO, "\n", 2);

    }
    return EXIT_SUCCESS;
}