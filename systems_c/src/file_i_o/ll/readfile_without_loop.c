#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Reading entire file without using loop.(using file metadata)\n");

  int fd = open("test", O_RDONLY);
  if (fd < 0) {
    perror("Failed to open the file.\n");
    exit(EXIT_FAILURE);
  }

  struct stat file_info;

  if (fstat(fd, &file_info) < 0) {
    perror("Failed to read metadata.\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  char *buffer = malloc(file_info.st_size + 1);

  if (buffer == NULL) {
    perror("Failed to create buffer.\n");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_read = read(fd, buffer, file_info.st_size);

  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    printf("File content : %s\n", buffer);
  } else {
    printf("Read nothing.\n");
  }

  free(buffer);
  close(fd);

  return EXIT_SUCCESS;
}
