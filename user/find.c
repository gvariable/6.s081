#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"


char *fmtname(char *path) {

  char *p;
  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; --p)
    ;
  ++p;
  return p;
}

void find(char *path, char *exp) {

  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    return;
  }

  switch (st.type) {
  case T_DIR: {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        
        if(!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        
        memcpy(p, de.name, sizeof(de.name));
        p[sizeof(de.name)] = 0;
        find(buf, exp);
    }
    break;
  }

  case T_DEVICE:
    break;

  case T_FILE: {
    char *basename = fmtname(path);
    if (!strcmp(basename, exp)) {
      printf("%s\n", path);
    }

    break;
  }
  }
  close(fd);

  return;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    exit(0);
  }

  if (argc == 2) {
    find(".", argv[1]);
  }

  if (argc == 3) {
    find(argv[1], argv[2]);
  }

  exit(0);
}