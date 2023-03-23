#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

int init_module(void);
void cleanup_module(void);

#define SUCCESS 0
#define DEVICE_NAME "charDevice"

/*
 * Global variables
 */

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

/* Constructor */
int init_module(void)
{ 
    printk(KERN_INFO "UNGS : Driver registrado\n");
    return 0;
}

/* Destructor */
void cleanup_module(void)
{ 
    printk(KERN_INFO "UNGS : Driver desregistrado\n");
}

static int device_open(struct inode *inode, struct file *filp)
{
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *filp)
{
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    return length;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UNGS");
MODULE_DESCRIPTION("Un primer driver");