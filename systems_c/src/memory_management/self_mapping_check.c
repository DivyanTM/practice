#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Memory map check.\n");

  int fd = -1;
  char buffer[256];

  fd = open("/proc/self/maps", O_RDONLY);

  if (fd < 0) {
    perror("Failed to open the file.\n");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_read;
  while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytes_read] = '\0';
    printf("%s", buffer);
  }

  close(fd);

  return EXIT_SUCCESS;
}
