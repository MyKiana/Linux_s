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

int main(int argc, char *argv[])
{
    int fd = open("/dev/test_drv", O_RDWR);
    if (fd < 0)
    {
        printf("open file failed !\n");
        return -1;
    }
    if(write(fd,TEST_STR,strlen(TEST_STR)) < 0)
    {
        printf("write error!\n");
        //return -1;
    }

/*    while (1)
    {
        sleep(60);
        break;
    }*/
    close(fd);
    return 0;
}