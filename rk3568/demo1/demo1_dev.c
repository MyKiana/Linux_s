//
// Created by Kiana on 2024/5/8.
//
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#define TEST_NAME "test_dev_drv"

/*static struct resource resources[] = {
    {
            .start = 0,
            .flags = IORESOURCE_IRQ,
    },
};*/

/*static struct platform_device test_dev = {
    .name = TEST_NAME,
    .num_resources = ARRAY_SIZE(resources),
    .resource = resources,
};*/
static struct platform_device test_dev = {
        .name = TEST_NAME,
};

static int __init _device_init(void)
{
    int err;

    err = platform_device_register(&test_dev);
    if(err < 0)
        printk("_device_init error!\n");
    return 0;
}

static void __exit _device_exit(void)
{
    platform_device_unregister(&test_dev);
}

module_init(_device_init);
module_exit(_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiana");