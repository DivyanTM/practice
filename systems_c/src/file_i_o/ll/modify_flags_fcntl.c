#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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
  printf("changing the flags using fcntl.\n");
  int fd = open("test", O_WRONLY | O_TRUNC | O_CREAT, 0644);

  if (fd < 0) {
    throwError("Failed to open the file.\n", errno, -1);
  }

  int flags = fcntl(fd, F_GETFL);
  if (flags < 0) {
    throwError("failed to read the flags.\n", errno, fd, -1);
  }

  if (flags & O_APPEND) {
    printf("Append mode is active\n");
  } else {
    printf("Append mode is inactive.\n");
  }

  flags |= O_APPEND;

  if (fcntl(fd, F_SETFL) < 0) {
    throwError("failed to modify flag.\n", errno, fd, -1);
  }

  printf("file mode has been modified.\n");

  close(fd);

  return EXIT_SUCCESS;
}
