#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void throwError(const char *errormessage, int error_no, ...) {
  va_list args;
  va_start(args, error_no);

  int fd_to_close;
  while ((fd_to_close = va_arg(args, int)) != -1) {
    close(fd_to_close);
  }

  va_end(args);

  errno = error_no;
  perror(errormessage);

  exit(EXIT_FAILURE);
}

int main(void) {
  printf("High performance file operations using mmap.\n");

  int fd = open("test", O_RDWR);
  if (fd < 0) {
    throwError("Failed to open the file.\n", errno, -1);
  }

  struct stat file_info;

  if (fstat(fd, &file_info) < 0) {
    throwError("Failed to read meta.\n", errno, fd, -1);
  }

  char *file_data =
      mmap(NULL, file_info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (file_data == MAP_FAILED) {
    throwError("Failed to map.\n", errno, fd, -1);
  }

  printf("file before changing : %s\n", file_data);

  file_data[0] = 'x';

  if (munmap(file_data, file_info.st_size) < 0) {
    throwError("munmap failed", errno, fd, -1);
  }

  close(fd);

  return EXIT_SUCCESS;
}
