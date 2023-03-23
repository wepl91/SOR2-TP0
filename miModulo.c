#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *inode, struct file *filp);
static int device_release(struct inode *inode, struct file *filp);
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char *tmp, size_t length, loff_t *offset);
void string_reverse(char *str);

#define SUCCESS 0
#define DEVICE_NAME "charDevice"
#define BUF_LEN 80

/*
 * Global variables
 */
static int Major;
static int Device_Open = 0;
static int msg_length = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
    };

/* Constructor */
int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0)
    {
        printk(KERN_INFO "Registrando driver con %d\n", Major);
        return Major;
    }
    printk(KERN_INFO "Driver registrado en %d\n", Major);
    return SUCCESS;
}

/* Destructor */
void cleanup_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "Driver desregistrado\n");
}

static int device_open(struct inode *inode, struct file *filp)
{
    if (Device_Open)
    {
        return -EBUSY;
    }

    Device_Open++;
    msg_Ptr = msg;

    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *filp)
{
    Device_Open--;
    module_put(THIS_MODULE);

    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    /*
     * If position is behind end of file there is nothing to read
     */
    if (*offset >= sizeof(msg))
    {
        return 0;
    }

    /*
     * If user try to read more than device has, read only as many bytes as device has
     */
    if (*offset + length > sizeof(msg))
    {
        msg_length = sizeof(msg) - *offset;
    }

    /*
     * Reverse string
     */
    string_reverse(msg);

    if (copy_to_user(buffer, msg + *offset, msg_length) != 0)
    {
        return -EFAULT;
    }

    *offset += msg_length;

    return msg_length;
}

static ssize_t device_write(struct file *filp, const char *tmp, size_t length, loff_t *offset)
{
    if (copy_from_user(msg, tmp, length))
    {
        return -EFAULT;
    }
    *offset += length;

    return length;
}

void string_reverse(char *str)
{
    /*
     * Get range
     */
    char *start = str;
    char *end = start + strlen(str) - 2; /* -2 for not to loose break line*/
    char tmp;

    /*
     * Skip null
     */
    if (str == 0)
    {
        return;
    }

    /*
     * Skip empty string
     */
    if (*str == 0)
    {
        return;
    }

    /*
     * Reverse string
     */
    while (end > start)
    {
        tmp = *start;
        *start = *end;
        *end = tmp;

        ++start;
        --end;
    }
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UNGS");
MODULE_DESCRIPTION("Un primer driver");