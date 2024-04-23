#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/version.h>


int kiana_flag = 0;

EXPORT_SYMBOL_GPL(kiana_flag);

static int __init _driver_init(void)
{
	
	printk("_driver_init%d\n",LINUX_VERSION_CODE);//#include <linux/version.h>
    printk("this process is %s,pid = %d\n",current->comm,current->pid);
	return 0;
}

static void __exit _driver_exit(void)
{
	printk("_driver_exit\n");
}

module_init(_driver_init);
module_exit(_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiana");