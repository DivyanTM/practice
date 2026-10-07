#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

int main(int argc, char *argv[]) {

  int read_file_d, write_file_d, open_flags;
  mode_t write_perms;
  ssize_t bytes_read, bytes_written;
  char buffer[BUFFER_SIZE];

  // handle command line arguments

  if (argc != 3) {
    fprintf(stderr, "usage : %s old-file new-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "usage : %s old-file new-file\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  // open file for reading
  read_file_d = open(argv[1], O_RDONLY);
  if (read_file_d < 0) {
    fprintf(stderr, "failed to read file %s : %s", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  // open file for writing
  open_flags = O_CREAT | O_WRONLY | O_TRUNC;
  write_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH;

  write_file_d = open(argv[2], open_flags, write_perms);
  if (write_file_d < 0) {
    fprintf(stderr, "Failed to open file %s : %s", argv[2], strerror(errno));
    close(read_file_d);
    exit(EXIT_FAILURE);
  }

  // write
  while ((bytes_read = read(read_file_d, buffer, BUFFER_SIZE)) > 0) {
    bytes_written = write(write_file_d, buffer, bytes_read);

    if (bytes_written != bytes_read) {
      if (bytes_written == -1) {
        fprintf(stderr, "failed to write to file %s : %s", argv[1],
                strerror(errno));
      } else {
        fprintf(stderr,
                "Fatal: Could not write whole buffer (wrote %zd out of %zd "
                "bytes)\n",
                bytes_written, bytes_read);
      }

      close(read_file_d);
      close(write_file_d);
      exit(EXIT_FAILURE);
    }
  }

  /*  Handle read errors */
  if (bytes_read == -1) {
    fprintf(stderr, "Error reading from '%s': %s\n", argv[1], strerror(errno));
    close(read_file_d);
    close(write_file_d);
    exit(EXIT_FAILURE);
  }

  /* Close file descriptors safely */
  if (close(read_file_d) == -1) {
    fprintf(stderr, "Error closing input file descriptor: %s\n",
            strerror(errno));
    close(write_file_d);
    exit(EXIT_FAILURE);
  }

  if (close(write_file_d) == -1) {
    fprintf(stderr, "Error closing output file descriptor: %s\n",
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
