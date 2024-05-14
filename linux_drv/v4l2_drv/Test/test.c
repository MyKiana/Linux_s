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
#include <string.h>

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

    int ret;
    if ((ret = ioctl(fd, VIDIOC_QUERYCAP, &cap)) < 0)
    {
        printf("ERR(%s):VIDIOC_QUERYCAP failed,ret = %d\n", __func__,ret);
        return -1;
    }

    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
        printf("v4l2 dev support capture\n");

    if(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
        printf("v4l2 dev support output\n");

    if(cap.capabilities & V4L2_CAP_VIDEO_OVERLAY)
        printf("v4l2 dev support overlay\n");

    if(cap.capabilities & V4L2_CAP_STREAMING)
        printf("v4l2 dev support streaming\n");

    if(cap.capabilities & V4L2_CAP_READWRITE)
        printf("v4l2 dev support read write\n");



/*
    struct v4l2_input input;
    input.index = 0;
    while (!ioctl(fd, VIDIOC_ENUMINPUT, &input))
    {
        printf("index = %d,input:%s\n",input.index,input.name);
        ++input.index;
    }
*/

    struct v4l2_input input;

    input.index = 0; //指定输入设备

    if (ioctl(fd, VIDIOC_S_INPUT, &input) < 0)
    {
        printf("ERR(%s):VIDIOC_S_INPUT failed\n", __func__);
        return -1;
    }


    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmtdesc.index = 0;

    while (!ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        printf("fmt:%s\n", fmtdesc.description);

        fmtdesc.index++;
    }


    struct v4l2_format v4l2_fmt;

    int width = 1920;
    int height = 1080;

    memset(&v4l2_fmt, 0, sizeof(struct v4l2_format));
    v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_fmt.fmt.pix.width = width; //宽度
    v4l2_fmt.fmt.pix.height = height; //高度
    v4l2_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //像素格式
    v4l2_fmt.fmt.pix.field = V4L2_FIELD_ANY;

    if (ioctl(fd, VIDIOC_S_FMT, &v4l2_fmt) < 0)
    {
        printf("ERR(%s):VIDIOC_S_FMT failed\n", __func__);
        return -1;
    }


    close(fd);
    return 0;
}