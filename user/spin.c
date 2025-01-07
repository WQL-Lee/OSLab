#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char * argv[]){
    int ret = fork();
    for(int i = 1; i < 10000; ++i){
        if(ret > 0){
            // parent
            write(1, "/", 1);
        }else{
            // child
            write(1, "\\", 1);
        }
    }

    exit(0);
}