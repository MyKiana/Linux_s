//
// Created by Kiana on 2024/4/27.
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TEST_STR "test com"

char *str = "test com";

int main(int argc, char *argv[])
{
    int fd = open("/dev/test_drv_demo3", O_RDWR);
    if (fd < 0)
    {
        printf("open file failed !\n");
        return -1;
    }

    ///z这里没有加上\0,会没有字符串结束标志///
    int count = write(fd,str,strlen(str) + 1);
    if(count < 0)
    {
        printf("write error! len = %d\n",(int)strlen(str));
        //return -1;
    }


    printf("count = %d    str = %d    STR = %d\n",count, (int)strlen(str), (int)strlen(TEST_STR));

    while(1)
    {
        sleep(10);
        break;
    }

/*    while (1)
    {
        sleep(60);
        break;
    }*/
    close(fd);
    return 0;
}