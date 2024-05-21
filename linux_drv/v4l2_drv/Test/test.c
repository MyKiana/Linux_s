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
#include <sys/mman.h>
#include <sys/poll.h>

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

    int width = 1280;
    int height = 720;

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

    struct v4l2_requestbuffers req;
    req.count = 30; //缓存数量
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0)
    {
        printf("ERR(%s):VIDIOC_REQBUFS failed\n", __func__);
        return -1;
    }


    struct v4l2_buffer v4l2_buffer;
    for(int i = 0; i < 30; i++)
    {
        memset(&v4l2_buffer, 0, sizeof(struct v4l2_buffer));
        v4l2_buffer.index = i; //想要放入队列的缓存
        v4l2_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buffer.memory = V4L2_MEMORY_MMAP;

        ret = ioctl(fd, VIDIOC_QBUF, &v4l2_buffer);
        if(ret < 0)
        {
            printf("Unable to queue buffer.\n");
            return -1;
        }
    }


    //打开设备
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
    {
        printf("ERR(%s):VIDIOC_STREAMON failed\n", __func__);
        return -1;
    }



    //等待
    struct pollfd poll_fds[1];
    poll_fds[0].fd = fd;
    poll_fds[0].events = POLLIN; //等待可读

    printf("waiting read\n");

    //出队
    poll(poll_fds, 30, 10000);
    struct v4l2_buffer buffer;
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    if (ioctl(fd, VIDIOC_DQBUF, &buffer) < 0)
    {
        printf("ERR(%s):VIDIOC_DQBUF failed, dropped frame\n", __func__);
        return -1;
    }

    printf("出队完成\n");

/*    //入队
    struct v4l2_buffer v4l2_buf;

    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    v4l2_buf.index = 0; //指定buf

    if (ioctl(fd, VIDIOC_QBUF, &v4l2_buf) < 0)
    {
        printf("ERR(%s):VIDIOC_QBUF failed\n", __func__);
        return -1;
    }




    //释放资源
    if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
    {
        printf("ERR(%s):VIDIOC_STREAMOFF failed\n", __func__);
        return -1;
    }*/


/*    for(int i = 0; i < 30; ++i)
        munmap(buf[i].addr, buf[i]->length);*/

    close(fd);
    return 0;
}