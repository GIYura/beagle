/* header section */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>   /* описаны макросы MAJOR/MINOR для получения адресов устройства */
#include <linux/uaccess.h>

#define DEV_MEM_SIZE    512
#define DEV_NUM_MAX     1

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt,__func__
 
static char m_device_buffer[DEV_MEM_SIZE];

/*
тип dev_t описан в linux/types.h
используется для хранения номеров устройств.
Разрядность 32 бита; 12 бит - мл. адрес; 20 - страший
*/
static dev_t m_device_number;

/*
структура описана в файле cdev.h
структура, представляющая символьное устройство
*/
static struct cdev m_cdev;

/*
структура описана в /include/linux/device.h
создает высокоуровневую абстракцию, которая позволяет
работать с устройствами основываясь что они делают, чем
как они работают
*/
static struct class* class_pcd;

/*
структура описана в /include/linux/device.h
базовая структура устройства.
на низком уровне каждой устройство в Linux это объект 
базовой структуры устройства.
*/
static struct device* device_pcd; 

/*
struct inode - описана в /linux/fs.h
Структура для хранения информации о файле или каталоге в файловой системе.
inode - класс (статическая информация о файле)
file - объект класса (динамическая информация о файле)
В структуре inode хранится тип dev_t (major/minor номера устройства).
Младший номер указывает на то, какое из устройств данного типа открыто open().

Структура file определена в /linux/fs.h.
Эта структура представляет файл устройства с точки ядра. Она создается ядром при
открытии файла.

NOTE: когда процесс пользователя открывает файл устройства (/dev/device-file),
VFS инициализирует поле f_op нового объекта адресом из inode.
*/

static int pcd_open(struct inode *inode, struct file *filp)
{
    pr_info("open was successful\n");
    return 0;
}

static int pcd_release(struct inode *inode, struct file *flip)
{
    pr_info("release was successful\n");
    return 0;
}

static ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld\n", *f_pos);
    
    /* adjust the count */
    if (*f_pos + count > DEV_MEM_SIZE)
        count  = DEV_MEM_SIZE - *f_pos;

    /* copy to user */
    if (copy_to_user(buff, &m_device_buffer[*f_pos], count))
        return -EFAULT;

    /* update current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /* return number of bytes successfully read */
    return count;
}

static ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("Write requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust the count */
    if ((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    if (!count)
    {
        pr_err("No space left on the device \n");
        return -ENOMEM;
    }

    /* copy from user */
    if (copy_from_user(&m_device_buffer[*f_pos], buff, count))
        return -EFAULT;

    /* update the current file postion */
    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /* return number of bytes successfully written */
    return count;
}

static loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    loff_t temp;

    pr_info("lseek requested \n");
    pr_info("Current value of the file position = %lld\n", filp->f_pos);

    switch (whence)
    {
        case SEEK_SET:
            if ((offset > DEV_MEM_SIZE) || (offset < 0))
                return -EINVAL;
            filp->f_pos = offset;
            break;

        case SEEK_CUR:
            temp = filp->f_pos + offset;
            if ((temp > DEV_MEM_SIZE) || (temp < 0))
                return -EINVAL;
            filp->f_pos = temp;
            break;

        case SEEK_END:
            temp = DEV_MEM_SIZE + offset;
            if ((temp > DEV_MEM_SIZE) || (temp < 0))
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
    int ret;

    /* dynamically alocate a device number */
    ret = alloc_chrdev_region(&m_device_number, 0, DEV_NUM_MAX, "custom-device");
    if (ret < 0)
    {
        pr_info("Failed to allocate device number\n");
        goto err_cdev_alloc;
    }

    pr_info("Device Number <major>:<minor> = %d:%d\n", MAJOR(m_device_number), MINOR(m_device_number));

    /* initialize the cdev struct with file operations 
    Инициализация структуры cdev (символьного утсройства) операциями над файлами, 
    которые поддерживает это устройство
    location: /fs/char_dev.c
    */
    cdev_init(&m_cdev, &m_fops);

    /* register a device with VFS 
    Добавление символьного устройства в ядро ч/з VFS.
    location: /fs/char_dev.c
    */
    m_cdev.owner = THIS_MODULE;
    ret = cdev_add(&m_cdev, m_device_number, DEV_NUM_MAX);
    if (ret < 0)
    {
        pr_info("Failed to add char driver\n");
        goto err_cdev_add;
    }
    
    /* create device class under /sys/class/<custom-class-name> */
    class_pcd = class_create(THIS_MODULE, "pcd_class");
    if (IS_ERR(class_pcd))
    {
        pr_err("Failed to create class\n");
        ret = PTR_ERR(class_pcd);
        goto err_cdev_class;
    }    

    /* populate device class with device info */
    device_pcd = device_create(class_pcd, NULL, m_device_number, NULL, "pcd");
    if (IS_ERR(device_pcd))
    {
        pr_err("Failed to create device\n");
        ret = PTR_ERR(class_pcd);
        goto err_cdev_create;
    }

    pr_info("Module init completed\n");

    return 0;

err_cdev_create: 
    class_destroy(class_pcd);

err_cdev_class: 
    cdev_del(&m_cdev);

err_cdev_add:
    unregister_chrdev_region(m_device_number, DEV_NUM_MAX);

err_cdev_alloc:
    return ret;
}

/* cleanup entry point */
static void __exit moduleExit(void)
{
    /* cleanup functions are called in reverse order as in init */
    device_destroy(class_pcd, m_device_number);
    class_destroy(class_pcd);
    cdev_del(&m_cdev);
    unregister_chrdev_region(m_device_number, DEV_NUM_MAX);

    pr_info("Module unloaded\n");
}

/* registration code */
module_init(moduleInit);
module_exit(moduleExit);

/* description section */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("Psudo-code of device driver");
MODULE_INFO(board,"Beaglebone black REV C");

