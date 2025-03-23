#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MSGSIZE 16
int
main(int argc, char * argv[]){
    sleep(10);
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);
    // printf("read : %s\n", buf);

    char * xargv[MAXARG];
    int xargc = 0;
    for(int i = 1; i < argc; ++i){
        xargv[xargc] = argv[i];
        // printf("argv: %s\n", argv[i]);
        ++xargc; 
    }

    char *p = buf;
    for(int i= 0; i < MSGSIZE; ++i){
        if(buf[i] == '\n'){
            int pid = fork();
            if(pid > 0){
                p = &buf[i+1];
                wait(0);
            }else{
                buf[i] = 0;
                xargv[xargc] = p;
                ++xargc;
    
                xargv[argc] = 0;
                ++xargc;
                exec(xargv[0], xargv);
                exit(0);
            }
        }
    }

    exit(0);
}