#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define NUMSIZE 36

void primes(int pr, int pw){
    char nums[NUMSIZE];
    read(pr, nums, NUMSIZE* sizeof(char));
    
    int idx = -1;
    for(int i = 0; i < NUMSIZE; ++i){
        if(nums[i] == '1'){
            idx = i;
            break;
        }
    }
    if(idx == -1) exit(0);
    printf("prime %d\n", idx);
    for(int i = idx; i < NUMSIZE; i += idx){
        nums[i] = '0';
    }
    
    int pid = fork();
    if(pid >0) {
        // 父进程
        write(pw, nums, NUMSIZE* sizeof(char));
        // wait(0);
    }else if (pid == 0){
        primes(pr, pw);
    }
}
int
main(int argc, char * argv[]){
    int fd[2];
    pipe(fd);
    char nums[NUMSIZE];
    nums[0] = '0', nums[1]= '0';
    // printf("%d\n", NUMSIZE);
    for(int i = 2; i < NUMSIZE;++i){
        nums[i] = '1';
    }

    int pid = fork();
    
    if(pid > 0){
        write(fd[1], nums, NUMSIZE* sizeof(char));
        wait(0);
    }else if(pid == 0){
        primes(fd[0], fd[1]);
        wait(0);
    }
    exit(0);
}