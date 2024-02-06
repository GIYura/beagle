#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#define DRIVER_NAME     "adxl345-i2c"
#define DRIVER_CLASS    "adxl345Class-i2c"

#define ADXL_ID_ADDR    0x00

#define DEV_MEM_SIZE    20 /* amount of bytes that can be read and write */

/* adapter = master 
declared /include/linux/i2c.h
i2c_adapter - struct to identify physical i2c bus
with the access algorithms to it.
*/
static struct i2c_adapter* adxl345_i2c_adapter = NULL;   /* I2C Adapter Structure */

/* client = slave 
declared /include/linux/i2c.h
i2c_client - single device connected to i2c bus
*/
static struct i2c_client* adxl345_i2c_client = NULL;     /* I2C Cient Structure */

/* Defines for device identification */ 
#define I2C_BUS_AVAILABLE       1           /* The I2C Bus available on the raspberry */
#define SLAVE_DEVICE_NAME       "ADXL345"   /* Device and Driver Name */
#define ADXL345_SLAVE_ADDRESS   0x53        /* ADXL345 I2C address */

/* драйвер устройства на шине 
I2C driver Structure that has to be added to linux
*/
static struct i2c_driver adxl345_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE
    }
};

/* I2C Board Info strucutre 
declared /include/linux/i2c.h
i2c_board_info - is used to build the table of devices present on bus.
*/
static struct i2c_board_info adxl345_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, ADXL345_SLAVE_ADDRESS)
};

/* Variables for Device and DeviceClass*/
static dev_t deviceNumber;          /* хранит номера major, minor */
static struct class* devClass;      /* определяет класс устройств */
static struct cdev device;          /* структура описывает символьное устройство */
static struct device* dev; 
static char m_buffer[DEV_MEM_SIZE]; /* буфер символьного устройства */

static u8 readId(void)
{
    return i2c_smbus_read_byte_data(adxl345_i2c_client, ADXL_ID_ADDR);
}

static ssize_t driver_write(struct file *filep, const char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("Write requested for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust the count */
    if ((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    if (!count)
    {
        pr_err("No space left on the device\n");
        return -ENOMEM;
    }

    /* copy from user */
    if (copy_from_user(&m_buffer[*f_pos], buffer, count))
        return -EFAULT;

    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    /* return successfully written number of bytes */
    return count; 
}

static ssize_t driver_read(struct file *filep, char __user *buffer, size_t count, loff_t *f_pos)
{
    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* read ID */
    u8 buff[DEV_MEM_SIZE];
    buff[0] = readId();
    
    /* adjust the count */
    if ((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;

    /* copy to user */
    if (copy_to_user(buffer, buff, count))
        return -EFAULT;

    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    return count;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *node, struct file *filep)
{
    printk("ADXL345: Open was called\n");
    return 0;
}

/**
 * @brief This function is called, when the device file is close
 */
static int driver_close(struct inode *inode, struct file *filep)
{
    printk("ADXL345: Close was called\n");
    return 0;
}

/* Map the file operations */
static struct file_operations devOps = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write,
};

/**
 * @brief function, which is called after loading module to kernel, do initialization there
 */
static int __init ModuleInit(void)
{
    int ret = -1;

    /* allocate a range of char device number */
    if (alloc_chrdev_region(&deviceNumber, 0, 1, DRIVER_NAME) < 0)
    {
        pr_err("ADXL345 Device Number could not be allocated!\n");
        return ret;
    }
    pr_info("ADXL345: Device Number: major %d: minor %d\n", MAJOR(deviceNumber), MINOR(deviceNumber));

    /* create device class */
    devClass = class_create(THIS_MODULE, DRIVER_CLASS);
    if (IS_ERR(devClass))
    {
        pr_err("ADXL345 Device Class can not be created!\n");
        ret = PTR_ERR(devClass);
        goto ClassError;
    }

    /* create device file */
    dev = device_create(devClass, NULL, deviceNumber, NULL, DRIVER_NAME);
    if (IS_ERR(dev))
    {
        pr_err("ADXL345 Can not create device file!\n");
        ret = PTR_ERR(dev);
        goto FileError;
    }

    /* initialize device file */
    cdev_init(&device, &devOps);

/* add char device to the VFS system 
NOTE: виртуальная файловая система (VFS) по типу файла пересылает файловые операции в соответсвующий канал.
Если файл, это файл устройсв, то VFS пересылает файловые операции в соответсвующий драйвер.
Регистрация драйвера в VFS состоит из двух этапов:
- cdev_init - заносит файловые операции в символьное устройство;
- cdev_add - регистрирует символьное устройство в VFS.
В /sys создается класс устройств, затем демон udev интерпритирует информцию об устройстве в 
пространстве пользователя (/dev).
*/
    if (cdev_add(&device, deviceNumber, 1) < 0)
    {
        pr_err("ADXL345 Registering of device to kernel failed!\n");
        goto AddError;
    }

    /* Get I2C controller
    доступен ли на шине 1 i2c контроллер
    */
    adxl345_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (adxl345_i2c_adapter != NULL)
    {
        /* instantiate an I2C client device */
        adxl345_i2c_client = i2c_new_client_device(adxl345_i2c_adapter, &adxl345_i2c_board_info);
        if (IS_ERR(adxl345_i2c_client))
        {
            pr_err("Failed to instantiate i2c client\n");
            goto AddError;
        }
        if (adxl345_i2c_client != NULL)
        {
            /* add driver to subsystem */
            if (i2c_add_driver(&adxl345_driver) != -1)
            {
                ret = 0;
            }
            else
            {
                pr_err("ADXL345 Can't add driver...\n");
                goto AddError;
            }
        }
        /* must call i2c_put_adapter() when done with returned i2c_adapter device */
        i2c_put_adapter(adxl345_i2c_adapter);
    }
    pr_info("ADXL345 Driver added!\n");

    return ret;

AddError:
    device_destroy(devClass, deviceNumber);
FileError:
    class_destroy(devClass);
ClassError:
    unregister_chrdev(deviceNumber, DRIVER_NAME); 
    return ret;
}

/**
 * @brief function, which is called when removing module from kernel
 * free alocated resources
 */
static void __exit ModuleExit(void)
{
    i2c_unregister_device(adxl345_i2c_client);
    i2c_del_driver(&adxl345_driver);
    cdev_del(&device);
    device_destroy(devClass, deviceNumber);
    class_destroy(devClass);
    unregister_chrdev_region(deviceNumber, 1);
}

module_init(ModuleInit);
module_exit(ModuleExit);

/* Meta Information */
MODULE_AUTHOR("Jura");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A driver for reading out ID ADXL345 Accelerometer");

