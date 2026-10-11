#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define CHUNK_SIZE (256 * 256 * 256)

void throwError(const char *error_message, int error_number, ...) {

  va_list(args);
  va_start(args, error_number);

  int fd_to_close;
  while ((fd_to_close = va_arg(args, int)) != -1) {
    close(fd_to_close);
  }
  errno = error_number;
  perror(error_message);

  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    if (argc == 2) {
      if (strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "usage : %s old-file new-file\n", argv[0]);
        exit(EXIT_SUCCESS);
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

  int src_fd, dest_fd;
  struct stat src_file_meta;

  if ((src_fd = open(src_path, O_RDONLY)) < 0) {
    throwError("failed to open the source file.\n", errno, -1);
  }

  if (fstat(src_fd, &src_file_meta) < 0) {
    throwError("Failed to read file info.\n", errno, src_fd, -1);
  }

  ssize_t file_size = src_file_meta.st_size;

  if (file_size == 0) {
    dest_fd = open(dest_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
      throwError("Failed to open the file.\n", errno, src_fd, -1);
    }
    close(dest_fd);
    close(src_fd);
    exit(EXIT_SUCCESS);
  }

  dest_fd = open(dest_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (dest_fd < 0) {
    throwError("Failed to open the file.\n", errno, src_fd, -1);
  }

  if ((ftruncate(dest_fd, file_size)) < 0) {
    throwError("Failed to truncate the destination file.\n", errno, src_fd,
               dest_fd, -1);
  }

  for (off_t offset = 0; offset < file_size; offset += CHUNK_SIZE) {
    ssize_t current_size =
        (file_size - offset > CHUNK_SIZE) ? CHUNK_SIZE : (file_size - offset);

    void *src_map =
        mmap(NULL, current_size, PROT_READ, MAP_SHARED, src_fd, offset);

    if (src_map == MAP_FAILED) {
      throwError("Failed to create map.\n", errno, src_fd, dest_fd, -1);
    }

    void *dest_map = mmap(NULL, current_size, PROT_WRITE | PROT_READ,
                          MAP_SHARED, dest_fd, offset);
    if (dest_map == MAP_FAILED) {
      throwError("Failed to create map.\n", errno, src_fd, dest_fd, -1);
    }

    memcpy(dest_map, src_map, current_size);

    if (msync(dest_map, current_size, MS_SYNC) < 0) {
      throwError("Failed to memcpy.\n", errno, src_fd, dest_fd, -1);
    }

    if (fdatasync(dest_fd) < 0) {
      throwError("Failed to fdatasync.\n", errno, src_fd, dest_fd, -1);
    }

    munmap(src_map, current_size);
    munmap(dest_map, current_size);
  }

  close(src_fd);
  close(dest_fd);

  printf("Successfully copied %zu bytes.\n", file_size);
  exit(EXIT_SUCCESS);
}
