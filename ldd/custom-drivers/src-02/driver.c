/* header section */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEV_MEM_SIZE 512

static char m_dev_buffer[DEV_MEM_SIZE];
static dev_t m_device_number;           /* stores device number */
static struct cdev m_cdev;              /* cdev variable */
static struct file_operations m_fops;   /* file operations for the driver */
 
/* init entry point */
static int __init moduleInit(void)
{
    int res;

    /* 1. Dynamically alocate a device number */
    res = alloc_chrdev_region(&m_device_number, 0, 1, "custom-device");
    if (res)
    {
        printk("Failed to allocate device number\n");
        return res;
    }

    /* 2. Initialize the cdev struct with file operations */
    cdev_init(&m_cdev, &m_fops);

    /* 3. Register a device with VFS */
    m_cdev.owner = THIS_MODULE;
    res = cdev_add(&m_cdev, m_device_number, 1);
    if (res)
    {
        printk("Failed to add char driver\n");
        return res;
    }

    return 0;
}

/* cleanup entry point */
static void __exit moduleExit(void)
{
    pr_info("Good bye world\n");
}

/* registration code */
module_init(moduleInit);
module_exit(moduleExit);

/* description section */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Psudo-code of device driver");
MODULE_INFO(board,"Beaglebone black REV C");

