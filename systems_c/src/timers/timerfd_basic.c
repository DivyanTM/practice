#include <bits/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  printf("timer fd basics\n");

  int tfd = timerfd_create(CLOCK_MONOTONIC, 0);

  if (tfd == -1) {
    perror("timerfd_create");
    exit(EXIT_FAILURE);
  }

  struct itimerspec its = {.it_value = {.tv_sec = 3},
                           .it_interval = {.tv_sec = 0}};

  if (timerfd_settime(tfd, 0, &its, NULL) == -1) {
    perror("timerfd_settime");
    close(tfd);
    exit(EXIT_FAILURE);
  }

  printf("Timer created, starting.....\n");
  uint64_t expiration;

  ssize_t n = read(tfd, &expiration, sizeof(expiration));

  if (n == -1) {
    perror("read");
    close(tfd);
    exit(EXIT_FAILURE);
  }

  printf("Timer expired\n");
  printf("Expiration count : %lu\n", (unsigned long)expiration);

  close(tfd);

  return EXIT_SUCCESS;
}
