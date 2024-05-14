#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#define TEST_NAME "test_dev_drv"
//#define OLD_METHOD
#define NEW_METHOD

static dev_t test_devno;
static unsigned int major = 0;
static unsigned int minor = 0;

struct cdev *my_cdev;

struct class *test_class = NULL;
//struct device *test_device = NULL;

int kiana_flag = 0;

EXPORT_SYMBOL_GPL(kiana_flag);

static int test_open(struct inode *inode,struct file *file)
{
    printk("test_open \n");
    return 0;
}

static int test_release(struct inode *inode, struct file *filp)
{
    printk("test_release");
    return 0;
}

static ssize_t test_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *off)
{
    int copy_count = 0;
    char *read_buf;
    printk("cnt = %d\n",(int)cnt);
    read_buf = (char *)kzalloc(cnt,GFP_KERNEL);
    copy_count = copy_from_user(read_buf, buf,cnt);
    printk("copy_from_user buff : %s \n",read_buf);
    kfree(read_buf);
    return 0;
}

void device_release(struct device *dev)
{
    printk("device_release dev name = %s \n",dev->init_name);
}

static struct file_operations test_operation = {
    .owner = THIS_MODULE,
    .open = test_open,
    .write = test_write,
    .release = test_release,
};

static int demo1_drv_probe(struct platform_device *pdev)
{
/*    struct resource *res;
    res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);*/
    // 创建设备
    if (device_create(test_class, NULL, test_devno, NULL, TEST_NAME, minor) < 0) {
        printk("-------------> demo1_drv_probe error! \n");
    }
    return 0;
}

static int demo1_drv_remove(struct platform_device *pdev)
{
/*    struct resource *res;
    int i = 0;
    res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
    if (!res)
        break;*/
    device_destroy(test_class, test_devno);
    return 0;
}

///平台驱动///
static struct platform_driver demo1_drv = {
    .probe      = demo1_drv_probe,
    .remove     = demo1_drv_remove,
    .driver     = {
            .name   = TEST_NAME,
    },
};

static int __init _driver_init(void)
{
	printk("_driver_init \nversion = %d\n",LINUX_VERSION_CODE);//#include <linux/version.h>
    printk("this process is %s,pid = %d\n",current->comm,current->pid);//#include <linux/sched.h>

    ///1.申请设备号///
    int err = 0;

    err = alloc_chrdev_region(&test_devno,0,1,TEST_NAME);//#include <linux/fs.h>

    if(err < 0)
    {
        printk("alloc_chrdev_region error! err = %d \n",err);
        return 0;
    }

    ///2.在 test_devno 获取设备号///
    major = MAJOR(test_devno);//#include <linux/kdev_t.h>
    minor = MINOR(test_devno);//#include <linux/kdev_t.h>

    printk("major = %d,minor = %d\n",major,minor);

    ///3.字符设备注册,注册成功后可以使用 cat /proc/devices 命令查看该目录，如果存在主设备号和名字相同的字符设备，说明注册成功了///
    ///-------老方法-------///
#ifdef OLD_METHOD
    err = register_chrdev(major,TEST_NAME,&test_operation);//#include <linux/cdev.h>
    printk("OLD_METHOD\n");
#endif
    ///-------新方法------///
#ifdef NEW_METHOD
    my_cdev = cdev_alloc();
    my_cdev->ops = &test_operation;
    my_cdev->owner = THIS_MODULE;
    cdev_init(my_cdev,&test_operation);
    err = cdev_add(my_cdev,test_devno,1);
    if(err < 0)
    {
        printk("cdev_add error!\n");
        return 0;
    }
    printk("NEW_METHOD\n");
#endif

    ///4.类注册 添加成功后会在/sys/class/目录生成 TEST_NAME 的类///
    test_class = kzalloc(sizeof(*test_class), GFP_KERNEL);//#include <linux/slab.h>
    test_class->owner = THIS_MODULE;
    test_class->name = TEST_NAME;
    //test_class->class_release = class_create_release;
    class_register(test_class);

    ///5.设备的注册 添加成功后会在/dev/ 下生成设备///
/*    test_device = kzalloc(sizeof(*test_device), GFP_KERNEL);//#include <linux/slab.h>
    //printk("test_device addr = %d\n",test_device);
    test_device->init_name = TEST_NAME;
    test_device->class = test_class;
    test_device->devt = test_devno;
    test_device->parent = NULL;
    test_device->driver_data = NULL;
    test_device->release = device_release;
    device_register(test_device);*/

    err = platform_driver_register(&demo1_drv);
    return 0;
}

static void __exit _driver_exit(void)
{
	printk("_driver_exit\n");
    //device_unregister(test_device);
    platform_driver_unregister(&demo1_drv);

    class_unregister(test_class);

    unregister_chrdev(major,TEST_NAME);

#ifdef OLD_METHOD
    unregister_chrdev_region(test_devno,1);//#include <linux/fs.h>
#endif
#ifdef NEW_METHOD
    cdev_del(my_cdev);
#endif
    kfree(test_class);
    //kfree(test_device);
}

module_init(_driver_init);
module_exit(_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiana");