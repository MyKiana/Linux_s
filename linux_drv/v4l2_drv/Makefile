#KERNEL_DIR=/lib/modules/5.15.0-100-generic/build/

KERNEL_DIR=/usr/src/linux-headers-5.15.0-105-generic/

ARCH=x86_64
CROSS_COMPILE=x86_64-linux-gnu-
export  ARCH  CROSS_COMPILE

obj-m := v4l2drv.o

v4l2drv-y :=v4l2_drv.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	gcc ./Test/test.c -o ./Test/test
.PHONY:clean
clean:
	$(MAKE)  -C $(KERNEL_DIR) M=$(CURDIR) clean
