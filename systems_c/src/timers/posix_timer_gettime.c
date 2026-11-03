#include <bits/time.h>
#include <bits/types/sigevent_t.h>
#include <bits/types/struct_itimerspec.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  printf("POSIX timer get time example\n");

  timer_t timer_id;

  struct sigevent sig = {.sigev_notify = SIGEV_NONE};

  if (timer_create(CLOCK_MONOTONIC, &sig, &timer_id) == -1) {
    perror("create timer");
    exit(EXIT_FAILURE);
  }

  printf("POSIX Timer created\n");

  struct itimerspec its = {.it_interval = {.tv_sec = 0},
                           .it_value = {.tv_sec = 10}};
  if (timer_settime(timer_id, 0, &its, NULL) == -1) {
    perror("timer settime");
    timer_delete(timer_id);
    exit(EXIT_FAILURE);
  }

  sleep(3);

  struct itimerspec current;

  if (timer_gettime(timer_id, &current) == -1) {
    perror("timer get time");
    exit(EXIT_FAILURE);
  }

  printf("remaining : %ld\n", current.it_value.tv_sec);
  printf("interval : %ld\n", current.it_interval.tv_sec);

  timer_delete(timer_id);

  return EXIT_SUCCESS;
}
