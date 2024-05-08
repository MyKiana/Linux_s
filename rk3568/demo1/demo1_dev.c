//
// Created by Kiana on 2024/5/8.
//
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define TEST_NAME "test_dev_drv"



static int __init _device_init(void)
{

    return 0;
}

static void __exit _device_exit(void)
{

}

module_init(_device_init);
module_exit(_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiana");