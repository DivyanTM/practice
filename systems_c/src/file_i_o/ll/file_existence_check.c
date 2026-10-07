#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("file existence check.\n");

  const char *filepath = "test";

  if (access(filepath, F_OK) == 0) {
    printf("file  %s exists.\n", filepath);
  } else {
    perror("file doesn't exist.\n");
    exit(EXIT_FAILURE);
  }

  if (access(filepath, R_OK) == 0) {
    printf("Read permission exist.\n");
  }

  if (access(filepath, W_OK) == 0) {
    printf("Write permission exist.\n");
  }

  if (access(filepath, X_OK) == 0) {
    printf("Execute permission exist.\n");
  }

  return EXIT_SUCCESS;
}
