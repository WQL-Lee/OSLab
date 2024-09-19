// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// # include "kernel/param.h"
// # define BUF_SIZE  512
// int 
// main(int argc, char * argv[]){
//     // get the argv from the command line
//     int index = 0;
//     int i =0;
//     for (; i < argc-1; ++i){
//         fprintf(1,"%s ", argv[i]);
//     }
//     fprintf(1, "%s\n", argv[i]);
//     char *cmd[MAXARG];
//     for (int i=1; i < argc; ++i){
//         cmd[index++]= argv[i];
//     }
    
//     i=0;
//     for (; i < index-1; ++i){
//         fprintf(1,"%s ", cmd[i]);
//     }
//     fprintf(1, "%s\n", cmd[i]);

//     char buf[BUF_SIZE];
//     char *p = buf;
//     char *c=0;
//     while(read(0, c, 1)==1){
//         if (*c== '\n'){
//             // execute the current command
//             p = 0;
//             if(fork()==0){
//                 cmd[index]= buf;
//                 fprintf(1, "buf: %s\n", buf);
//                 // the child execute the command
//                 exec(argv[1], cmd);
//                 exit(0);
//             }
//             wait(0);
//             p = buf; // ready for execute next command
//         }else{
//             *p++ = *c;
//         }
//     }
//     exit(0);
// }


/**
 * @file xargs.c UNIX xargs 的简易实现
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    char* args[MAXARG+1];
    int index = 0;
    for (int i=1; i<argc; ++i) {
        args[index++] = argv[i];
    }

    char buf[BUF_SIZE];
    char *p = buf;
    while (read(0, p, 1) == 1) {
        if ((*p) == '\n') {
            *p = 0;
            int pid;
            if ((pid = fork()) == 0) {
                // child
                // exec only one arg
                args[index] = buf;
                exec(argv[1], args);
                fprintf(2, "exec %s failed\n", argv[1]);
                exit(0);
            }
            // parent
            wait(0);
            p = buf;
        } else {
            ++p;
        }
    }
    exit(0);
}
