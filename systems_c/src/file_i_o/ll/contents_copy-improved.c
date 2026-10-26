#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void throwError(const char *errormessage, int erro_no, ...) {
  va_list(args);
  va_start(args, erro_no);

  int fd_to_close;
  while ((fd_to_close = va_arg(args, int)) != -1) {
    close(fd_to_close);
  }
  va_end(args);
  errno = erro_no;
  perror(errormessage);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  // printf("file copy improved : error handling and edge cases.\n");
  if (argc != 3) {
    if (argc == 2) {
      if (strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage : %s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
      }
      fprintf(stderr, "%s : missing destination file operand after %s.\n",
              argv[0], argv[1]);

      fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
      exit(EXIT_FAILURE);
    } else if (argc == 1) {
      fprintf(stderr, "%s : missing file operand.\n", argv[0]);
      fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  const char *src_path = argv[1];
  const char *dest_path = argv[2];

  int src_fd = -1;
  int dest_fd = -1;
  struct stat src_file_meta;
  struct stat dest_file_meta;
  char *temp_template = NULL;

  // OPEN THE SOURCE FILE
  if ((src_fd = open(src_path, O_RDONLY)) < 0) {
    throwError("Failed to open the source file", errno, -1);
  }

  // READ THE SOURCE META DATA
  if (fstat(src_fd, &src_file_meta) < 0) {
    throwError("Failed to read source file meta data", errno, src_fd, -1);
  }

  // ENSURE SOURCE IS REGULAR FILE
  if (!S_ISREG(src_file_meta.st_mode)) {
    throwError("Source is not a regular file.", errno, src_fd, -1);
  }

  // PREVENT SELF COPY
  if (stat(dest_path, &dest_file_meta) == 0) {
    if (src_file_meta.st_dev == dest_file_meta.st_dev &&
        src_file_meta.st_ino == dest_file_meta.st_ino) {
      throwError("Source and destination are same file", errno, src_fd, -1);
    }
  }

  ssize_t file_size = src_file_meta.st_size;

  temp_template = malloc(strlen(dest_path) + 8);
  if (!temp_template) {
    throwError("Memory allocation failed", errno, src_fd, -1);
  }

  sprintf(temp_template, "%s.XXXXXX", dest_path);

  dest_fd = mkstemp(temp_template);
  if (dest_fd < 0) {
    free(temp_template);
    throwError("Failed to create temporary destination file", errno, src_fd,
               dest_fd, -1);
  }

  return EXIT_SUCCESS;
}
