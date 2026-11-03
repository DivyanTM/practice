#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  printf("Nanosleep remaining param example..\n");

  struct timespec req = {.tv_sec = 10, .tv_nsec = 0};
  struct timespec rem;

  if (nanosleep(&req, &rem) == -1) {
    if (errno == EINTR) {
      printf("sleep interrupted\n");
      printf("Remaining time : %ld sec,%ld nanosecond\n", rem.tv_sec,
             rem.tv_nsec);
    }
  } else {
    perror("nanosleep");
  }

  return EXIT_SUCCESS;
}
