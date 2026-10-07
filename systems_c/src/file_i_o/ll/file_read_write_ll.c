#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int fd;
  const char *message = "written by low level sys calls.\n";
  printf("file read write low level system calls.\n");

  fd = open("test", O_RDWR | O_CREAT | O_APPEND, 0644);
  if (fd < 0) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  ssize_t byets_written = write(fd, message, strlen(message));
  if (byets_written < 0) {
    perror("Failed to write.\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  lseek(fd, 0, SEEK_SET);

  char buff[256];
  ssize_t bytes_read;

  while ((bytes_read = read(fd, buff, sizeof(buff) - 1)) > 0) {
    buff[bytes_read] = '\0';
    printf("%s", buff);
  }

  if (bytes_read < 0) {
    perror("Failed to read.\n");
    exit(EXIT_FAILURE);
  }

  close(fd);

  return EXIT_SUCCESS;
}
