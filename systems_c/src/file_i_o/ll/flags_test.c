#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  printf("flags test program,might be deleted later.\n");

  int fd = open("test", O_WRONLY | O_TRUNC | O_CREAT, 0644);

  if (fd < 0) {
    fprintf(stderr, "Failed to open the file : %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  int flags = fcntl(fd, F_GETFL);

  if (flags < 0) {
    fprintf(stderr, "Failed to open the file : %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("flags of %d : %d.\n", fd, flags);

  printf("flag value of write only : %d.\n", O_WRONLY);

  printf("flag value of read only : %d.\n", O_RDONLY);

  printf("flag value of read write mode : %d.\n", O_RDWR);

  return EXIT_SUCCESS;
}
