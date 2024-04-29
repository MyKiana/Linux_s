//
// Created by Kiana on 2024/4/29.
//

#include "test_io.h"

//sudo EXTERNAL_URL=119.23.59.142 yum install -y gitlab-ce
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd = 0;
    fd = open("test.txt",O_RDWD,666);
    if(fd < 0)
        printf("not open!\n");


    close(fd);
    return 0;
}
