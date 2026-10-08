#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  printf("output redirection using dup2 , implementing ./script > file 2>&1 "
         "programattically.\n");

  int file_mode = O_RDWR | O_TRUNC | O_CREAT;
  mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = open("output.log", file_mode, perms);

  if (fd < 0) {
    fprintf(stderr, "Failed to open file : %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (dup2(fd, STDOUT_FILENO) < 0) {
    fprintf(stderr, "Failed to redirect stdout :: %s.\n", strerror(errno));
    close(fd);
    exit(EXIT_FAILURE);
  }

  if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0) {
    fprintf(stderr, "Failed to redirect stderr : %s.\n", strerror(errno));
    close(fd);
    exit(EXIT_FAILURE);
  }

  close(fd);

  printf("1. normal ouput written to stdout.\n");

  fflush(stdout);

  fprintf(stderr, "1.Failure message sent to stderr.\n");

  printf(
      "3. They share the same offset, so they do not overwrite each other!\n");

  return EXIT_SUCCESS;
}
