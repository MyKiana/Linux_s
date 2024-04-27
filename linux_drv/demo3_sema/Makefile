#KERNEL_DIR=/lib/modules/5.15.0-100-generic/build/

KERNEL_DIR=/usr/src/linux-headers-5.15.0-105-generic/

ARCH=x86_64
CROSS_COMPILE=x86_64-linux-gnu-
export  ARCH  CROSS_COMPILE

obj-m := demo3.o
modules-objs := demo3.o vermagic.o


all:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules
	
.PHONY:clean
clean:
	$(MAKE)  -C $(KERNEL_DIR) M=$(CURDIR) clean
