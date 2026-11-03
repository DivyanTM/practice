#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  printf("Nanosleep basic example\n");

  struct timespec req = {.tv_sec = 5, .tv_nsec = 500000000};

  printf("starting...\n");
  nanosleep(&req, NULL);
  printf("completed.\n");

  return EXIT_SUCCESS;
}
