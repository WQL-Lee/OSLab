#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
# define MSGLEN 16
int
main (int argc, char * argv[])
{
    int fd[2];
    pipe(fd);
    int ret = fork();
    char * msg = "this is a message";
    char buf[MSGLEN];
    memset(buf, 0, sizeof(buf));

    if(ret > 0){
        // 父进程
        write(fd[1], msg, MSGLEN);

        read(fd[0], buf, MSGLEN);
        printf("%d: received pong\n", getpid());
    }else{
        // 子进程
        read(fd[0], buf, MSGLEN);
        printf("%d: received ping\n", getpid());
        write(fd[1], msg, MSGLEN);
    }
    exit(0);
}