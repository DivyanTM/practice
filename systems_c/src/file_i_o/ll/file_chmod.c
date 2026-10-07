#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  const char *file_path = "test";
  printf("Change file mod.\n");

  if (access(file_path, F_OK) < 0) {
    perror("file doesn't exist.\n");
    exit(EXIT_FAILURE);
  }

  if (chmod(file_path, 0644) < 0) {
    perror("chmod failed.\n");
  } else {
    printf("chmod success.\n");
  }

  return EXIT_SUCCESS;
}
