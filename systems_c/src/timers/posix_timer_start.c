#include <bits/time.h>
#include <bits/types/sigevent_t.h>
#include <bits/types/struct_itimerspec.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  printf("POSIX Timer start example\n");
  timer_t timer_id;

  struct sigevent sev = {.sigev_notify = SIGEV_NONE};

  if (timer_create(CLOCK_MONOTONIC, &sev, &timer_id) == -1) {
    perror("timer_create");
    exit(EXIT_FAILURE);
  }

  printf("POSIX Timer created successfully\n");

  struct itimerspec its = {.it_value = {.tv_sec = 5},
                           .it_interval = {.tv_sec = 0}};

  if (timer_settime(timer_id, 0, &its, NULL) == -1) {
    perror("timer_settime");
    timer_delete(timer_id);
    exit(EXIT_FAILURE);
  }

  printf("Timer started\n");
  sleep(6);

  if (timer_delete(timer_id) == -1) {
    perror("timer_delete");
    exit(EXIT_FAILURE);
  }

  printf("POSIX Timer deleted successfully\n");

  return EXIT_SUCCESS;
}
