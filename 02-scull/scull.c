#include <linux/module.h>	//for module
#include <linux/kernel.h>	//for printk

#include <linux/moduleparam.h>  //for module extern parameter

#include <linux/fs.h>	//for register char dev. 

#include <linux/cdev.h> // for cdev add
#include <linux/slab.h> // for kmalloc
void clean_module(void);
/*******************************************************************************
* module parameter  charp int uint  short ushort array
*******************************************************************************/
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
* char device 
*******************************************************************************/
// define a kind of scull device implement 
struct scull_cdev {
	int quantum;
	int qset;
	unsigned long size;
	struct cdev cdev;
};
struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.open = NULL,
	.read = NULL,
	.write = NULL,
	.release = NULL,
};
//define a pubilic  device ID
static dev_t dev_id;
//define a structure point to scull char devices.
static struct scull_cdev * scull_cdevice;

/*******************************************************************************
* module init function
*******************************************************************************/
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
// get dev number register
//
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
			result = alloc_chrdev_region(&dev_id, 0, scull_number, "scull_dyn");
		}
		else {	//manual regester the device
			dev_id = MKDEV(scull_major, scull_minor);
			result = register_chrdev_region(dev_id, scull_number, "scull_sta");
		}
		if ( result < 0 ) {
			printk(KERN_WARNING "scull: can not get a major");
		}else{
			scull_major = MAJOR(dev_id);
			printk(KERN_WARNING "Major:%d Minor:%d", scull_major, MINOR(dev_id));
		}
	}
	{	// allocate  memery for scull cher device.
		int i;
		scull_cdevice = kmalloc( sizeof(struct scull_cdev) * scull_number, GFP_KERNEL);
		if ( scull_cdevice == NULL ) {
			printk(KERN_ERR "kmalloc: no memery for scull char device");
			goto failed_exit;
		}
		// initilize the allocate memery
		memset(scull_cdevice, 0 ,  sizeof(struct scull_cdev) * scull_number);
		// config and initilize char device 
		for (i = 0; i< scull_number; i++) {
		//config variables
			scull_cdevice[i].quantum = scull_quantum;
			scull_cdevice[i].qset = scull_qset;
			scull_cdevice[i].size = 0;
		//initilize the cdev structure
			cdev_init( &scull_cdevice[i].cdev, &scull_fops);
			scull_cdevice[i].cdev.owner = THIS_MODULE;
			scull_cdevice[i].cdev.ops = &scull_fops;
		//add char device.
			dev_id = MKDEV(scull_major, scull_minor + i);
			result = cdev_add( &scull_cdevice[i].cdev, dev_id, 1);
			if (result  < 0) {
				printk(KERN_ERR "add cdev device failed");
				break;
			} else {
				printk(KERN_NOTICE "cdev major:%d minor:%d", MAJOR(dev_id), MINOR(dev_id));
			}
		}
	}
	return result;
failed_exit:	// how to deal with errors
	// clean up 
	clean_module();
	return result;
}
void clean_module(void)
{
	printk(KERN_WARNING "Clean up Module init");

// Last-2. free malloc memery
	kfree(scull_cdevice);
// Last-1. unregister cdev
	unregister_chrdev_region(dev_id, scull_number);
}
/*******************************************************************************
* module exit function
*******************************************************************************/
static void __exit scull_exit(void)
{
	int i;
//  delete scull char devices cdev_del
	for ( i = 0; i < scull_number; i++) {
		cdev_del(&scull_cdevice[i].cdev);	
	}
	clean_module();
	printk("Goodbye kernel\n");
}

module_init(scull_init);	//module install
module_exit(scull_exit);	//module uninstall
MODULE_AUTHOR("Stephen Fu");	//author
MODULE_LICENSE("GPL");			//license
