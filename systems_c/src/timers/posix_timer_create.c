#include <bits/time.h>
#include <bits/types/sigevent_t.h>
#include <bits/types/timer_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  printf("POSIX TIMER CREATE\n");

  timer_t timer_id;
  struct sigevent sev = {.sigev_notify = SIGEV_NONE};

  if (timer_create(CLOCK_MONOTONIC, &sev, &timer_id) == -1) {
    perror("timer_create");
    exit(EXIT_FAILURE);
  }

  printf("POSIX timer created successfully.\n");

  if (timer_delete(timer_id) == -1) {
    perror("timer_delete");
    exit(EXIT_FAILURE);
  }

  printf("POSIX timer delete successfull.\n");

  return EXIT_SUCCESS;
}
