#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int getArgs(char *argv[]) {

  static char buf[512], *p = buf;
  int argc = 0;

  while (read(0, p++, sizeof(char)) == sizeof(char)) {
  }
  *p++ = 0;
  for (char *start = buf, *end = buf; *end != 0; end++) {
    if (*end == ' ' || *end == '\t' || *end == '\n') {
      argv[argc++] = start;
      start = end + 1;
      *end = 0;
    }
  }
  return argc;
}

void execCmd(char *argv[]) {
  int pid = fork();
  if (pid > 0) {
    wait(0);
  } else {
    exec(argv[0], argv);
  }
}

int main(int argc, char *argv[]) {

  if (argc == 1) {
    exit(0);
  }

  char *argvCmd[MAXARG];
  int argcCmd = 0;
  char *argvAppend[MAXARG];
  int argcAppend = getArgs(argvAppend);
  int idx = 1;
  int n = 0;

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
      execCmd(argvCmd);
    }

    if (!argcAppend % n) {
      for (int j = 0; j < argcAppend % n; ++j) {
        argvCmd[argcCmd++] = argvAppend[argcAppend / n * n + j];
      }
      argvCmd[argcCmd++] = 0;
      execCmd(argvCmd);  
    }

  } else {
    for (int i = 0; i < argcAppend; ++i) {
      argvCmd[argcCmd++] = argvAppend[i];
    }
    execCmd(argvCmd);
  }
  exit(0);
}