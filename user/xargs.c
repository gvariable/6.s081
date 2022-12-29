#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

  if (argc == 1) {
    exit(0);
  }

  char *argvCmd[MAXARG];
  int argcCmd = 0;
  char *argvAppend[MAXARG];
  int argcAppend = 0;
  int idx = 1;
  int n = 0;

  char buf[512], *p = buf;

  while (read(0, p++, sizeof(char)) == sizeof(char)) {
  }
  *p++ = 0;
  for (char *start = buf, *end = buf; *end != 0; end++) {
    if (*end == ' ' || *end == '\t' || *end == '\n') {
      argvAppend[argcAppend++] = start;
      start = end + 1;
      *end = 0;
    }
  }

  if (argc > 1) {
    if (!strcmp(argv[1], "-n")) {
      n = atoi(argv[2]);
      idx += 2;
    }
  }

  while (idx < argc) {
    argvCmd[argcCmd++] = argv[idx++];
  }

  if (n) {
    for (int i = 0; i < argcAppend / n; ++i) {
      int argcCmdTmp = argcCmd;
      for (int j = 0; j < n; ++j) {
        argvCmd[argcCmdTmp++] = argvAppend[i * n + j];
      }

      int pid = fork();
      if (pid > 0) {
        wait(0);
      } else {
        exec(argvCmd[0], argvCmd);
      }
    }

    for (int j = 0; j < argcAppend % n; ++j) {
        argvCmd[argcCmd++] = argvAppend[argcAppend / n * n + j];
    }
    argvCmd[argcCmd++] = 0;
    int pid = fork();
      if (pid > 0) {
        wait(0);
      } else {
        exec(argvCmd[0], argvCmd);
      }

  } else {
    for (int i = 0; i < argcAppend; ++i) {
      argvCmd[argcCmd++] = argvAppend[i];
    }

    int pid = fork();
    if (pid > 0) {
      wait(0);
    } else {
      exec(argvCmd[0], argvCmd);
    }
  }
  exit(0);
}