//
// Created by Kiana on 2024/5/14.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#define PATH_NAME "/dev/video0"

int main(void)
{
    int fd;
    struct v4l2_capability cap;
    fd = open(PATH_NAME,O_RDWR);
    if(fd < 0)
    {
        printf("ERR(%s):failed to open %s\n", __func__,PATH_NAME);
    }
    printf("fd = %d\n",fd);

    if (ioctl(fd, VIDIOC_QUERYCAP, cap) < 0)
    {
        printf("ERR(%s):VIDIOC_QUERYCAP failed\n", __func__);
        return -1;
    }

    close(fd);
    return 0;
}