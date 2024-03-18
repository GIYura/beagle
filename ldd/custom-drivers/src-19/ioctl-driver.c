#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#include "ioctl-driver.h"

#define DEV_NUMBER_MAX  1

#define DRIVER_NAME     "ioctl-driver"
#define DRIVER_CLASS    "ioctl-driver-class"

/* Variables for Device and DeviceClass*/
static dev_t devNumber;         /* хранит номера major, minor */
static struct class* devClass;  /* определяет класс устройств */
static struct cdev device;      /* структура описывает символьное устройство */
static struct device* dev;      /* базовая структура девайса; /include/linux/device.h */ 

static int testValue = 22;

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *inode, struct file *filp)
{
    pr_info("IOCTL: Driver open called!\n");
    return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *inode, struct file *filp)
{
    pr_info("IOCTL: Driver close called!\n");
    return 0;
}

static long int ioctl_test(struct file* f, unsigned cmd, unsigned long arg)
{
    struct test tst;

    switch(cmd)
    {
        case WR_VALUE:
            if (copy_from_user(&testValue, (int32_t *) arg, sizeof(testValue))) 
                pr_err("IOCTL: Failed to copy from user!\n");
            else
                pr_info("IOCTL: Updated value: %d\n", testValue);
        break;
    
        case RD_VALUE:
            if (copy_to_user((int32_t *) arg, &testValue, sizeof(testValue))) 
                pr_err("IOCTL: Failed to copy data to user!\n");
            else
                pr_info("IOCTL: The value was copied!\n");
            break;

        case GREETER:
            if (copy_from_user(&tst, (struct test *) arg, sizeof(tst))) 
                pr_err("IOCTL: Failed to copy data from user!\n");
            else
                pr_info("IOCTL: Test value %d greets %s\n", tst.value, tst.name);
            break;  
    }
    return 0;
}

static struct file_operations devFops = 
{
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .unlocked_ioctl = ioctl_test
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) 
{
    int ret = -1;

    /* allocate a range of char device number */
    if (alloc_chrdev_region(&devNumber, 0, DEV_NUMBER_MAX, DRIVER_NAME) < 0)
    {
        pr_err("IOCTL: Failed to allocate device number!\n");
        return ret;
    }
    pr_info("IOCTL: Device number: major %d: minor %d\n", MAJOR(devNumber), MINOR(devNumber));

    /* create device class; visible in /sys/class */
    devClass = class_create(THIS_MODULE, DRIVER_CLASS);
    if (IS_ERR(devClass))
    {
        pr_err("IOCTL: Failed to create  device class!\n");
        ret = PTR_ERR(devClass);
        goto ClassError;
    }

    /* create device file; create device under /dev */
    dev = device_create(devClass, NULL, devNumber, NULL, DRIVER_NAME);
    if (IS_ERR(dev))
    {
        pr_err("IOCTL: Failed to create device file!\n");
        ret = PTR_ERR(dev);
        goto FileError;
    }

    /* initialize device file */
    cdev_init(&device, &devFops);
    
    /* add char device to the system */
    if (cdev_add(&device, devNumber, DEV_NUMBER_MAX) < 0)
    {
        pr_err("IOCTL: Failed to add device to kernel!\n");
        goto AddError;
    }

    ret = 0;

    pr_info("IOCTL: Driver added!\n");

    return ret;

AddError:
    device_destroy(devClass, devNumber);
FileError:
    class_destroy(devClass);
ClassError:
    unregister_chrdev(devNumber, DRIVER_NAME); 
    return ret;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void)
{
    cdev_del(&device);
    device_destroy(devClass, devNumber);
    class_destroy(devClass);
    unregister_chrdev_region(devNumber, DEV_NUMBER_MAX);
    pr_info("IOCTL: Driver deleted!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("IOCTL driver example");

