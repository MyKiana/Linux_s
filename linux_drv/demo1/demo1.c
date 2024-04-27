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

#define TEST_NAME "test_drv"
//#define OLD_METHOD
#define NEW_METHOD

static dev_t test_devno;
static unsigned int major = 0;
static unsigned int minor = 0;

struct cdev *my_cdev;

struct class *test_class;
struct device *test_device;

int kiana_flag = 0;

EXPORT_SYMBOL_GPL(kiana_flag);

int test_open(struct inode *inode,struct file *file)
{
    printk("test_open \n");
    return 0;
}

static struct file_operations test_operation = {
    .owner = THIS_MODULE,
    .open = test_open
};

static int __init _driver_init(void)
{
	printk("_driver_init%d\n",LINUX_VERSION_CODE);//#include <linux/version.h>
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
    test_device = kzalloc(sizeof(*test_device), GFP_KERNEL);//#include <linux/slab.h>
    test_device->init_name = TEST_NAME;
    test_device->class = test_class;
    test_device->devt = test_devno;
    test_device->parent = NULL;
    test_device->driver_data = NULL;
    device_register(test_device);
    return 0;
}

static void __exit _driver_exit(void)
{
	printk("_driver_exit\n");
    device_unregister(test_device);

    class_unregister(test_class);

    unregister_chrdev(major,TEST_NAME);

#ifdef OLD_METHOD
    unregister_chrdev_region(test_devno,1);//#include <linux/fs.h>
#endif
#ifdef NEW_METHOD
    cdev_del(my_cdev);
#endif
}

module_init(_driver_init);
module_exit(_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiana");