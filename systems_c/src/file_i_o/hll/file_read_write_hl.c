#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("file handling using high level api.\n");

  FILE *file = fopen("test", "w");

  if (file == NULL) {
    perror("Failed to open file.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(file, "Hello World\n");
  fprintf(file, "High level API\n");
  fclose(file);

  file = fopen("test", "r");
  if (file == NULL) {
    perror("error opening file for reading\n");
    exit(EXIT_FAILURE);
  }

  char buffer[256];

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    printf("Read : %s", buffer);
  }

  fclose(file);

  return EXIT_SUCCESS;
}
