#include <linux/module.h>	//for module
#include <linux/kernel.h>	//for printk

#include <linux/moduleparam.h>  //for module extern parameter

#include <linux/fs.h>	//for register char dev. 

#include <linux/cdev.h> // for cdev add

static int scull_major = 0;
static int scull_minor = 0;
static int scull_number= 1;
static int scull_quantum = 4000;	//default value
static int scull_qset = 1000;
static char *message=NULL;
static int a_num = 8; 
static int mesg_array[10]={0};

module_param(scull_major, int, S_IRUGO); //declare extern parmeters
module_param(scull_minor, int, S_IRUGO);
module_param(scull_number, int, S_IRUGO);
module_param(scull_quantum, int,S_IRUGO);
module_param(message, charp, S_IRUGO);
module_param_array(mesg_array, int, &a_num, S_IRUGO); //comma

/*******************************************************************************
* module init function
*******************************************************************************/
dev_t dev_num;
static int __init scull_init(void)
{
	int result = 0;
//1.printk
	printk(KERN_WARNING "hello kernel\n");
	{
//2. Test module parameter
		printk(KERN_WARNING "Default Scull_major:%d",	scull_major);
		printk(KERN_WARNING "Default Scull_minor: %d",	scull_minor);
		printk(KERN_WARNING "Default Scull_number:%d", scull_number);
		printk(KERN_WARNING "Quantum:%d - Qset:%d",scull_quantum, scull_qset);
		printk(KERN_WARNING "Message:%s", message);
		while ( --a_num )	
			printk(KERN_WARNING "mesg array %d:%d", a_num, mesg_array[a_num]);
	}
//3. char device  register
/*** key function***************************************************************
* 1. old version register char device
*	register_chrdev     unregister_chrdev
* 2. regiter now
*	----register_chrdev_region    //static by manual	MKDEV(major,minor)
*	----alloc_chrdev_region			//dynamic 
*	----unregister_chrdev_region  //
*	----cdev_alloc
*	----cdev_init
*	----cdev_add
*	----cdev_del
*******************************************************************************/
	{
		if (scull_major == 0 )	{ // We not get a device major, dynamic regester 
			result = alloc_chrdev_region(&dev_num, 0, scull_number, "scull_dyn");
		}
		else {	//manual regester the device
			dev_num = MKDEV(scull_major, scull_minor);
			result = register_chrdev_region(dev_num, scull_number, "scull_sta");
		}
		if ( result < 0 ) {
			printk(KERN_WARNING "scull: can not get a major");
		}else{
			printk(KERN_WARNING "Major:%d Minor:%d", MAJOR(dev_num), MINOR(dev_num));
		}
	}
	return result;
}
/*******************************************************************************
* module exit function
*******************************************************************************/
static void __exit scull_exit(void)
{
//1. unregister cdev
	unregister_chrdev_region(dev_num, scull_number);

	printk("Goodbye kernel\n");
}

module_init(scull_init);	//module install
module_exit(scull_exit);	//module uninstall
MODULE_AUTHOR("Stephen Fu");	//author
MODULE_LICENSE("GPL");			//license
