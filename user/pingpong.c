#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p[2];

  if (pipe(p) < 0) {
    fprintf(2, "cannot pipe\n");
  };
  char buffer[10];

  if (fork() == 0) {
    // child process received ping
    int n = read(p[0], buffer, sizeof(buffer));
    buffer[n] = 0;
    close(p[0]);

    printf("%d: received %s\n", getpid(), buffer);
    write(p[1], "pong", sizeof("pong"));
    close(p[1]);
  } else {
    write(p[1], "ping", sizeof("ping"));
    close(p[1]);

    wait(0);
    int n = read(p[0], buffer, sizeof(buffer));
    buffer[n] = 0;

    printf("%d: received %s\n", getpid(), buffer);
    close(p[0]);

  }

  exit(0);
}