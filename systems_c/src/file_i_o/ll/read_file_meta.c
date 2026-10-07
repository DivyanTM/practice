#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("file meta data.\n");

  int fd;
  fd = open("test", O_RDONLY);

  if (fd < 0) {
    perror("Failed to open file.\n");
    exit(EXIT_FAILURE);
  }

  struct stat file_info;

  if (fstat(fd, &file_info) < 0) {
    perror("unable to access file.\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("----------------file info-----------------\n");
  printf("Size : %lld bytes\n", (long long)file_info.st_size);

  printf("Inodes : %llu\n", (unsigned long long)file_info.st_ino);

  printf("links : %lu\n", (unsigned long)file_info.st_nlink);

  printf("file permissions : %o\n", file_info.st_mode & 0777);

  close(fd);
  return EXIT_SUCCESS;
}
