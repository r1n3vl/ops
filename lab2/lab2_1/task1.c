#include <asm-generic/errno-base.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#define ARRAY_LENGTH 32
struct Lines {
  int NN;
  char content[64];
  int count;
};

ssize_t bulk_write(int fd, const void *buf, size_t nbytes) {
  ssize_t written_bytes;
  size_t count = 0;
  while (count < nbytes) {
    written_bytes = write(fd, (char *)buf + count, nbytes - count);
    if (written_bytes == 0) {
      break;
    }
    if (written_bytes == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("write");
      return -1;
    }
    count += written_bytes;
  }
  return (ssize_t)count;
}
ssize_t bulk_read(int fd, void *buf, size_t nbytes) {
  ssize_t bytesread;
  size_t count = 0;
  while (count < nbytes) {
    bytesread = read(fd, (char *)buf + count, nbytes - count);
    if (bytesread == -1 && errno != EAGAIN) {
      perror("read");
      return -1;
    }
    if (bytesread == 0) {
      break;
    }
    count += bytesread;
  }
  return (ssize_t)count;
}

int createstr(struct Lines *right, char *buff) {

  char *content;
  long num = 0;
  char *number = strtok(buff, ":");
  if (number != NULL) {
    num = strtol(number, NULL, 10);
    if (num >= 0 && num <= 31) {
      if ((content = strtok(NULL, "\n")) == NULL) {
        return EXIT_FAILURE;
      }
      right[num].NN = num;

      right[num].count += 1;
      strcpy(right[num].content, content);
    }

  } else {
    {
      return EXIT_FAILURE;
    }
  }
  while (1) {
    number = strtok(NULL, ":");
    if (number != NULL) {
      num = strtol(number, NULL, 10);
      if (num >= 0 && num <= 31) {
        if ((content = strtok(NULL, "\n")) == NULL) {
          return EXIT_FAILURE;
        }
        right[num].NN = num;
        right[num].count += 1;
        strcpy(right[num].content, content);
      } else {
        const char *msg = "Granny got carried away\n";
        write(STDERR_FILENO, msg, strlen(msg));
        strtok(NULL, "\n");
      }

    } else {

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

void print_str(struct Lines *line, int fd) {

  ftruncate(fd, 0);
  lseek(fd, 0, SEEK_SET);
  for (int i = 0; i < 32; i++) {
    if (line[i].count >= 1) {
      size_t len = 63 - strlen(line[i].content);
      write(fd, line[i].content, strlen(line[i].content));
      for (int i = 0; (size_t)i < len; i++) {
        write(fd, " ", 1);
      }

      write(fd, "\n", 1);

      char lin[19];
      if (line[i].NN <= 9) {
        snprintf(lin, 19, "Step 0%1d saved\n", line[i].NN);
      } else {
        snprintf(lin, 19, "Step %1d saved\n", line[i].NN);
      }
      write(STDOUT_FILENO, lin, strlen(lin));
      // sleep(1);

    } else {
      for (int j = 0; j < 63; j++) {
        write(fd, " ", 1);
      }
      write(fd, "\n", 1);
    }
  }
}
char *babka(char *argv) {
  char cwd[256];
  getcwd(cwd, 256);
  // printf("%s\n", cwd);
  char *granny =
      malloc(sizeof("There is no granny in ") + strlen(cwd) + 2 + sizeof(argv));
  strcpy(granny, "There is no granny in ");
  strcat(granny, cwd);
  strcat(granny, "/");
  strcat(granny, argv);
  // printf("%s\n", granny);
  return granny;
}

char *getfp(char *argv) {
  char cwd[256];
  getcwd(cwd, 256);
  char *f = malloc(strlen(cwd) + strlen(argv) + 2);
  strcpy(f, cwd);
  // printf("%s", f);
  strcat(f, "/");
  // printf("%s", f);
  strcat(f, argv);
  // printf("%s", f);

  return f;
}
int get_descriptors(struct pollfd fds[5], char **argv, int argc) {

  int n = 0;

  for (int i = 2; i < argc && n < 5; i++) {

    char *f = getfp(argv[i]);

    char *granny = babka(argv[i]);
    // printf("%s", granny);
    // printf("KAAAABAAANHSDJABKDJKASBDKB\n");

    int fd = open(f, O_RDONLY | O_NONBLOCK);

    struct stat st;
    lstat(argv[i], &st);

    if (fd == -1 || !S_ISFIFO(st.st_mode)) {
      write(STDERR_FILENO, granny, strlen(granny));
      write(STDERR_FILENO, "\n", 1);
      return -1;
    }
    fds[n].fd = fd;
    fds[n].events = POLLIN;
    fds[n].revents = 0;

    n++;

    free(f);
    free(granny);
  }
  // printf("KAAAABAAAN\n");

  return n;
}
int main(int argc, char **argv) {
  int fd;
  if (argc < 3 || argc > 7) {
    fprintf(stderr, "Usage: %s recipe fifo1 [fifo2 ... fifo5]\n", argv[0]);
    return EXIT_FAILURE;
  }

  char pending[5][128];
  size_t used[5] = {0};
  off_t offset;
  char *err = "This book is not a recipe!\n";
  // ssize_t readbytes;
  // if (argc < 7) {
  //   perror("oshibka");
  //   return EXIT_FAILURE;
  // }
  struct pollfd fds[5];
  struct Lines *right = calloc(ARRAY_LENGTH, sizeof(struct Lines));
  if (access(argv[1], F_OK) == -1) {
    fd = open(argv[1], O_RDWR | O_NONBLOCK | O_CREAT, 0644);
    if (fd == -1) {
      perror("open");
      return EXIT_FAILURE;
    }
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < 63; j++) {
        write(fd, " ", 1);
      }
      write(fd, "\n", 1);
    }
  } else {
    fd = open(argv[1], O_RDWR | O_NONBLOCK);
  }

  offset = lseek(fd, 0, SEEK_END);
  if (offset != 2048) {
    write(STDERR_FILENO, err, strlen(err));
    return EXIT_FAILURE;
  }
  // fd2 = open(argv[2], O_RDONLY);

  char buf[5][2048];
  int n = get_descriptors(fds, argv, argc);
  // printf("KAAAABAAAN\n");
  if (n == 0) {
    fprintf(stderr, "No readable event devices (check permissions)\n");
    return 1;
  }
  while (1) {
    int active = 0;

    for (int j = 0; j < n; j++) {
      if (fds[j].fd >= 0) {
        active++;
      }
    }

    if (active == 0) {
      break;
    }
    int ret = poll(fds, n, 3000);

    if (ret == -1) {
      if (errno == EINTR) {
        continue;
      }

      perror("poll");
      return EXIT_FAILURE;
    }

    if (ret == 0) {
      const char *msg = "Grannies are pouring more tea…\n";
      write(STDOUT_FILENO, msg, strlen(msg));
      continue;
    }

    for (int j = 0; j < n; j++) {
      if (fds[j].fd < 0) {
        continue;
      }
      // printf("KAAAABAAAN\n");

      if (fds[j].revents & (POLLERR | POLLNVAL)) {
        close(fds[j].fd);
        fds[j].fd = -1;
        continue;
      }

      if (!(fds[j].revents & (POLLIN | POLLHUP))) {
        continue;
      }
      while (1) {
        ssize_t readdenbytes = read(fds[j].fd, buf[j], sizeof(buf[j]) - 1);

        if (readdenbytes == -1) {
          if (errno == EINTR) {
            continue;
          }

          if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
          }

          perror("read");
          close(fds[j].fd);
          fds[j].fd = -1;
          break;
        }

        if (readdenbytes == 0) {
          printf("Granny from %s finished dictating\n", argv[j + 2]);
          close(fds[j].fd);
          fds[j].fd = -1;
          break;
        }

        for (ssize_t k = 0; k < readdenbytes; k++) {
          if (buf[j][k] == '\n') {
            pending[j][used[j]] = '\0';

            createstr(right, pending[j]);
            print_str(right, fd);

            used[j] = 0;
          } else {
            if (used[j] >= sizeof(pending[j]) - 1) {
              fprintf(stderr, "Message too long\n");
              return EXIT_FAILURE;
            }

            pending[j][used[j]++] = buf[j][k];
          }
        }
      }
    }
  }
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    return EXIT_FAILURE;
  }
  size_t length = sb.st_size;
  char *buffer = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (buffer == MAP_FAILED) {
    perror("mmap");
  }
  char *found = strstr(buffer, "raw~berries");
  int fo = 1;
  while (found != NULL) {
    memcpy(found, "raspberries", 11);
    found = strstr(buffer, "raw~berries");
    fo++;
  }
  char* bufd = malloc(sizeof(buffer));
  strcpy(bufd, buffer);
  msync(buffer, length, MS_SYNC);
  munmap(buffer, length);
  char msg[50];
  int k = 0;
  int b = 0;
  snprintf(msg, 48, "Princess Joanna's stains cleaned: %d\n", fo);
  write(STDOUT_FILENO, msg, sizeof(msg));
  char *fal = strtok(bufd, "\n");
  if (strstr(fal, "                                                          ") == NULL) {
    char lin[4];
    if (b <= 9) {
      snprintf(lin, 4, "0%1d:", b);
    } else {
      snprintf(lin, 3, "%1d:", b);
    }
    write(STDOUT_FILENO, lin, strlen(lin));
    write(STDOUT_FILENO, fal, 64);
    write(STDOUT_FILENO, "\n", 2);
    b++;
  } else {
    k++;
    b++;
  }
  for (int i = 0; i < 31; i++) {
    fal = strtok(NULL, "\n");
    if (strstr(fal, "                                              ") == NULL) {
      // printf("%s",fal);
      char lin[4];
      if (b <= 9) {
        snprintf(lin, 4, "0%1d:", b);
      } else {
        snprintf(lin, 4, "%1d:", b);
      }
      write(STDOUT_FILENO, lin, strlen(lin));
      write(STDOUT_FILENO, fal, 64);
      write(STDOUT_FILENO, "\n", 2);
      b++;
    } else {
      k++;
      b++;
    }
  }
  // printf("AAAAAAAAAAAAA%d", k);
  if (k == 32) {
    write(STDOUT_FILENO, "Cake is ready\n", sizeof("Cake is ready\n"));
  } else {
    snprintf(msg, 50, "Missing %d steps, the cake will collapse\n", k);
    write(STDOUT_FILENO, msg, sizeof(msg));
  }
  for (int i = 0; i < n; i++) {
    if (fds[i].fd >= 0) {
      close(fds[i].fd);
    }
  }
  if (close(fd) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }
  // msync(buffer, length, MS_SYNC);
  // munmap(buffer, length);
  free(right);
  free(bufd);
  return EXIT_SUCCESS;
}