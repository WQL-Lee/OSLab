#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int cmp_last(char *path, char *name){
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    memmove(buf, p, strlen(p));

    if (memcmp(p, name, strlen(p))==0){
        return 0;
    }else{
        return -1;
    }
}

void
find(char *path, char * pattern)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(cmp_last(path, pattern)==0){
        printf("%s\n", path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){

      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }else{

        if (cmp_last(buf, pattern)==0){
            printf("%s\n", buf);
        }
        if (st.type == T_DIR && cmp_last(buf, ".")!=0 && cmp_last(buf, "..") !=0){
            find(buf, pattern);
        }
      }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{

    if (argc <3){
        fprintf(2, "You need to input 3 argv!");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
