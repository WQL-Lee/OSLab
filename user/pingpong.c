#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

    if (argc != 1){
        fprintf(2, "Pingpong program doesn't need to specify the argv!\n");
        exit(1);
    }

    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    int ret = fork();
    char cont[512];
    if (ret ==0){
        // child
        close(p1[1]); // close the write side
        read(p1[0], cont, 1);
        close(p1[0]);
        int child_pid = getpid();
        fprintf(2, "%d: received ping\n", child_pid);
        
        close(p2[0]);
        write(p2[1], "C", 1);
        close(p2[1]);
        // exit(0);
    }
    else if (ret > 0){
        // parent
        close(p1[0]); // close the read side
        write(p1[1], "P", 1); // write something to the pipe
        close(p1[1]);

        close(p2[1]); // close the write side
        read(p2[0], cont, 1);
        close(p2[0]);
        int parent_pid = getpid();
        fprintf(2, "%d: received pong\n", parent_pid);

    }else{
        fprintf(2, "Error: Can not fork a process!");
        exit(1);
    }
    exit(0);
}