#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int parse_int(const char* arg) {
    const char* p = arg;
    for ( ; *p ; p++ ) {
        if ( *p < '0' || *p > '9' ) {
            return -1;
        }
    }
    return atoi(arg);
}

int
main(int argc, char *argv[])
{

  if (argc != 2){
    fprintf(2, "Sleep program has not specify the argv!\n");
    exit(1);
  }
  int time = parse_int(argv[1]);
  if (time< 0) {
    printf("Error argument\n");
  }

  int ticks = atoi(argv[1]);
  sleep(ticks);
  exit(0);
}
