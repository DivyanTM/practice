#include <bits/time.h>
#include <bits/types/sigevent_t.h>
#include <bits/types/struct_itimerspec.h>
#include <bits/types/timer_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t timer_expired = 0;

void handleSignal(int signal) { timer_expired = 1; }

int main(void) {
  printf("POSIX Timer Signal\n");

  timer_t timer_id;
  struct sigaction sa = {0};
  sa.sa_handler = handleSignal;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  struct sigevent sev = {.sigev_notify = SIGEV_SIGNAL, .sigev_signo = SIGUSR1};

  if (timer_create(CLOCK_MONOTONIC, &sev, &timer_id) == -1) {
    perror("timer_create");
    exit(EXIT_FAILURE);
  }

  puts("Timer crated succssfully");

  struct itimerspec its = {.it_interval = {.tv_sec = 0},
                           .it_value = {.tv_sec = 10}};

  if (timer_settime(timer_id, 0, &its, NULL) == -1) {
    perror("timer set timer");
    timer_delete(timer_id);
    exit(EXIT_FAILURE);
  }

  printf("Waiting foer timer\n");

  while (!timer_expired) {
    pause();
  }

  timer_delete(timer_id);

  return EXIT_SUCCESS;
}
