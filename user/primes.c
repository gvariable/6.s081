#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SIZE 35

// parent close rfd and wfd
// child read data by rfd from parent and write data as another parent
int primes(int rfd) {

  int prime = 2, readVal = 2;
  if (read(rfd, &prime, sizeof(int)) == 0) {
    close(rfd);
    return 0;
  }
  printf("prime %d\n", prime);

  int p[2];
  if (pipe(p) < 0) {
    fprintf(2, "pipe allocated error\n");
    close(rfd);
    exit(0);
  }

  int pid = fork();
  if (pid > 0) {
    close(p[0]);

    while (read(rfd, &readVal, sizeof(int))) {
      if (readVal % prime) {
        write(p[1], &readVal, sizeof(readVal));
      }
    }
    close(rfd);
    close(p[1]);
    wait(0);
  } else if (pid == 0) {
    close(p[1]);
    primes(p[0]);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fprintf(2, "%s only accept one param\n", argv[0]);
    exit(1);
  }

  int p[2];
  if (pipe(p) < 0) {
    fprintf(2, "pipe allocated error\n");
    exit(1);
  }

  if (fork() > 0) {
    close(p[0]);

    for (int i = 2; i <= SIZE; ++i) {
      write(p[1], &i, sizeof(i));
    }

    close(p[1]);
    wait(0);
  } else {
    close(p[1]);
    primes(p[0]);
  }

  exit(0);
}
