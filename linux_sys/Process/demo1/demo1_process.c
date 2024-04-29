//
// Created by Kiana on 2024/4/29.
//

#include "demo1_process.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    pid_t pid;
    printf("he1 = %d\n",getpid());
    if((pid = fork()) < 0) {
        printf("fork error!\n");
    }else if(pid == 0){
        printf("children pid = %d, ppid---> %d\n",(int)getpid(),(int)getppid());
        pid_t x;
        printf("he2 = %d\n",getpid());
        if((x = fork()) < 0)
            printf("fork error2!\n");
        else if(x == 0){
            printf("children2 pid = %d, ppid---> %d\n",(int)getpid(),(int)getppid());
        }else{
            printf("ppid2 = %d\n",(int)getpid());
        }

    }else{
        printf("ppid = %d\n",(int)getpid());
    }

    printf("----------------->pid = %d\n",(int)getpid());

    exit(0);
}