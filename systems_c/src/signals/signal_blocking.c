#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t shutdown_requested = 0;

void sig_handler(int sig) { shutdown_requested = 1; }

int main(void) {
  printf("signal blocking\n");

  struct sigaction sa = {0};

  sigset_t block_set;
  sigset_t pending_set;
  sigset_t old_set;

  sa.sa_handler = sig_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGTERM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  sigemptyset(&block_set);
  sigaddset(&block_set, SIGTERM);

  if (sigprocmask(SIG_BLOCK, &block_set, &old_set) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  printf("PID : %d \n", getpid());
  printf("SIGTERM is blocked,starting critical operations...\n");

  sleep(10);

  printf("Critical operations finished.\n");

  if (sigpending(&pending_set) == -1) {
    perror("sigpending");
    exit(EXIT_FAILURE);
  }

  if (sigismember(&pending_set, SIGTERM)) {
    printf("SIGTERM is pending\n");
  } else {
    printf("No pending sigterms");
  }

  printf("restoring previous signal maska\n");

  if (sigprocmask(SIG_SETMASK, &old_set, NULL) == -1) {
    perror("sigprocmask error");
    exit(EXIT_FAILURE);
  }

  if (shutdown_requested) {
    printf("shutdown requested\n");
    printf("shutdown complete\n");
  } else {
    printf("No shutdown requested\n");
  }

  return EXIT_SUCCESS;
}
