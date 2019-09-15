#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

static int num=0;
static int array[10]={0};
static char * mesg="Hello, kernel"; 

module_param(num, int, S_IRUGO);
module_param(mesg, charp, S_IRUGO);
module_param_array(array, int, &num, S_IRUGO);

static int __init helloworld_init(void)
{
	printk(KERN_ALERT "Hello kernel\n");
	printk(KERN_ALERT "num:%d\n",num);
	printk(KERN_ALERT "mesg:%s\n",mesg);
	return 0;
}


static void __exit helloworld_exit(void)
{
	printk(KERN_ALERT "Good bye, kernel\n");
}


module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_AUTHOR("Stephen");
MODULE_LICENSE("GPL");
MODULE_VERSION("4.18.0");
MODULE_DESCRIPTION("TEST");
//MODULE_DEVICE_TABLE(char,"VIRTUAL MACHINE");
MODULE_ALIAS("TEST HELLO");
