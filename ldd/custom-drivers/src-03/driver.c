/* header section */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__
 
#define MEM_SIZE_MAX_PCDEV_1    1024
#define MEM_SIZE_MAX_PCDEV_2    512
#define MEM_SIZE_MAX_PCDEV_3    1024
#define MEM_SIZE_MAX_PCDEV_4    512

#define DEV_NUM_MAX             4

/* permission codes */
#define RDONLY  0x01
#define WRONLY  0X10
#define RDWR    0x11

/* device's memory */
static char m_device_buffer_1[MEM_SIZE_MAX_PCDEV_1];
static char m_device_buffer_2[MEM_SIZE_MAX_PCDEV_2];
static char m_device_buffer_3[MEM_SIZE_MAX_PCDEV_3];
static char m_device_buffer_4[MEM_SIZE_MAX_PCDEV_4];

/* device private data */
struct pcdev_private_data
{
    char* buffer;
    unsigned int size;
    const char* serialNumber;
    int perm;
    struct cdev cdev;
#if 0
    struct spinlock_t pcdev_spin_lock;  /* not suitable */
#endif
    struct mutex pcdev_lock;
};

/* driver private data */
struct pcdrv_data
{
    int devCount;                   /* supported device number */
    dev_t device_number;            /* stores device number */
    struct class* class_pcd;
    struct device* device_pcd; 
    struct pcdev_private_data pcdev_data[DEV_NUM_MAX];
};

static struct pcdrv_data m_pcdrv_data = 
{
    .devCount = DEV_NUM_MAX,
    .pcdev_data = {
                        [0] = {
                            .buffer = m_device_buffer_1,
                            .size = MEM_SIZE_MAX_PCDEV_1,
                            .serialNumber = "PCDEV-01",
                            .perm = RDONLY,
                          },

                        [1] = {
                            .buffer = m_device_buffer_2,
                            .size = MEM_SIZE_MAX_PCDEV_2,
                            .serialNumber = "PCDEV-02",
                            .perm = WRONLY,
                          },

                        [2] = {
                            .buffer = m_device_buffer_3,
                            .size = MEM_SIZE_MAX_PCDEV_3,
                            .serialNumber = "PCDEV-03",
                            .perm = RDWR,
                          },

                        [3] = {
                            .buffer = m_device_buffer_4,
                            .size = MEM_SIZE_MAX_PCDEV_4,
                            .serialNumber = "PCDEV-04",
                            .perm = RDWR,
                          },
                  }
};

/*
macro F_MODE_READ and FMODE_WRITE defined in /linux/fs.h
*/
static int check_permission(int dev_perm, int acc_mode)
{
    if (dev_perm == RDWR)
        return 0;
    
    /* ensures readonly access */
    if ( (dev_perm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)) )
        return 0;
    
    /* ensures writeonly access */
    if ( (dev_perm == WRONLY) && ((acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ)) )
        return 0;

    return -EPERM;
}

static int pcd_open(struct inode *inode, struct file *filp)
{
    int ret = 0;
    int minor = 0;
    struct pcdev_private_data *pcdev_data;

    /* find out on which device file open was attempted by the user */
    minor = MINOR(inode->i_rdev);
    pr_info("Minor access = %d\n", minor);

    /* get device's private data structure */
    pcdev_data = container_of(inode->i_cdev, struct pcdev_private_data, cdev);

    /* supply device private data to other methods of the driver */
    filp->private_data = pcdev_data;

    /* check permission */
    ret = check_permission(pcdev_data->perm, filp->f_mode);

    (ret) ? pr_info("Failed to open\n") : pr_info("Open successfull\n");

    return ret;
}

static int pcd_release(struct inode *inode, struct file *flip)
{
    pr_info("release was successful\n");
    return 0;
}

static ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
    int max_size = pcdev_data->size;

    mutex_lock(&pcdev_data->pcdev_lock);

    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld\n", *f_pos);
    
    /* adjust the count */
    if (*f_pos + count > max_size)
        count = max_size - *f_pos;

    /* copy to user */
    if (copy_to_user(buff, pcdev_data->buffer + (*f_pos), count))
    {
        mutex_unlock(&pcdev_data->pcdev_lock);
        return -EFAULT;
    }
    /* update current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    mutex_unlock(&pcdev_data->pcdev_lock);

    /* return number of bytes successfully read */
    return count;
}

static ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
    int max_size = pcdev_data->size;

    mutex_lock(&pcdev_data->pcdev_lock);

    pr_info("Write requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust the count */
    if ((*f_pos + count) > max_size)
        count = max_size - *f_pos;

    if (!count)
    {
        pr_err("No space left on the device \n");
        mutex_unlock(&pcdev_data->pcdev_lock);
        return -ENOMEM;
    }

    /* copy from user */
    if (copy_from_user(pcdev_data->buffer + (*f_pos), buff, count))
    {
        mutex_unlock(&pcdev_data->pcdev_lock);
        return -EFAULT;
    }

    /* update the current file postion */
    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n",*f_pos);

    mutex_unlock(&pcdev_data->pcdev_lock);

    /* return number of bytes successfully written */
    return count;
}

static loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
    int max_size = pcdev_data->size;
    loff_t temp;

    pr_info("lseek requested \n");
    pr_info("Current value of the file position = %lld\n", filp->f_pos);

    switch (whence)
    {
        case SEEK_SET:
            if ((offset > max_size) || (offset < 0))
                return -EINVAL;
            filp->f_pos = offset;
            break;

        case SEEK_CUR:
            temp = filp->f_pos + offset;
            if ((temp > max_size) || (temp < 0))
                return -EINVAL;
            filp->f_pos = temp;
            break;

        case SEEK_END:
            temp = max_size + offset;
            if ((temp > max_size) || (temp < 0))
                return -EINVAL;
            filp->f_pos = temp;
            break;

        default:
            return -EINVAL;
    }
    
    pr_info("New value of the file position = %lld\n", filp->f_pos);

    return filp->f_pos;
}

static struct file_operations m_fops =    /* file operations for the driver */
{
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .release = pcd_release,
    .llseek = pcd_lseek,
    .owner = THIS_MODULE
};


/* init entry point */
static int __init moduleInit(void)
{
    int ret = 0;
    u8 i = 0;

    /* Dynamically alocate a device number */
    ret = alloc_chrdev_region(&m_pcdrv_data.device_number, 0, DEV_NUM_MAX, "pcdevs");
    if (ret < 0)
    {
        pr_info("Failed to allocate device number\n");
        goto err_cdev_alloc;
    }

    /* create device class under /sys/class */
    m_pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(m_pcdrv_data.class_pcd))
    {
        pr_err("Failed to create class\n");
        ret = PTR_ERR(m_pcdrv_data.class_pcd);
        goto err_cdev_class;
    }
    
    for (i = 0; i < DEV_NUM_MAX; i++)
    {
        pr_info("Device number <major>:<minor> = %d:%d\n", MAJOR(m_pcdrv_data.device_number + i), MINOR(m_pcdrv_data.device_number + i));

#if 0
        spin_lock_init(&pcdrv_data.pcdev_data[i].pcdev_spin_lock);
#endif
        mutex_init(&m_pcdrv_data.pcdev_data[i].pcdev_lock);

        /* Initialize the cdev struct with file operations */
        cdev_init(&m_pcdrv_data.pcdev_data[i].cdev, &m_fops);

        /* Register a device with VFS */
        m_pcdrv_data.pcdev_data[i].cdev.owner = THIS_MODULE;
        ret = cdev_add(&m_pcdrv_data.pcdev_data[i].cdev, m_pcdrv_data.device_number + i, 1);
        if (ret < 0)
        {
            pr_info("Failed to add char driver\n");
            goto err_cdev_add;
        }
    
        /* populate device class with device info */
        m_pcdrv_data.device_pcd = device_create(m_pcdrv_data.class_pcd, NULL, m_pcdrv_data.device_number + i, NULL, "pcdev-%d", i);
        if (IS_ERR(m_pcdrv_data.device_pcd))
        {
            pr_err("Failed to create device\n");
            ret = PTR_ERR(m_pcdrv_data.device_pcd);
            goto err_cdev_create;
        }
    }

    pr_info("Module init completed\n");

    return 0;

err_cdev_create: 
err_cdev_class:

    for (; i>= 0; i--)
    {
        device_destroy(m_pcdrv_data.class_pcd, m_pcdrv_data.device_number + i);
        cdev_del(&m_pcdrv_data.pcdev_data[i].cdev);
    }
    class_destroy(m_pcdrv_data.class_pcd);

err_cdev_add:
    unregister_chrdev_region(m_pcdrv_data.device_number, DEV_NUM_MAX);

err_cdev_alloc:
    return ret;
}

/* cleanup entry point */
static void __exit moduleExit(void)
{
    u8 i = 0;
    for (i = 0; i < DEV_NUM_MAX; i++)
    {
        device_destroy(m_pcdrv_data.class_pcd, m_pcdrv_data.device_number + i);
        cdev_del(&m_pcdrv_data.pcdev_data[i].cdev);
    }
    class_destroy(m_pcdrv_data.class_pcd);
    unregister_chrdev_region(m_pcdrv_data.device_number, DEV_NUM_MAX);

    pr_info("Module unloaded\n");
}

/* registration code */
module_init(moduleInit);
module_exit(moduleExit);

/* description section */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Psudo-code of device driver for multiple devices");
MODULE_INFO(board,"Beaglebone black REV C");

