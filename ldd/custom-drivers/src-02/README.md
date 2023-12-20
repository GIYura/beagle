### DRIVER TEST

#### HOST

1. Compile driver code
```
make
```

2. Copy driver to the target
```
make copy
```

#### TARGET

3. Load the driver
```
sudo insmod driver.ko
```

4. Make sure driver loaded successfully
```
dmesg | tail -2
ls -l /dev/pcd - device file should be added
ls -l /sys/class/pcd_class - link to pcd should be added
```

5. Run
```
echo hello target > /dev/pcd - write 'hello' string to driver
cat /dev/pcd - read the string from driver
```

6. Unload the driver
```
sudo rmmod driver.ko
```

**NOTE:** change device file access permission:
```
sudo chown [user:group] /path-to-file
```

**NOTE:**
 
After creating a char device we need access to it from the user space.
To do this, a device node should be added under **/dev**.

This can be done in two ways:
- manually add device node (outdated)
```
mknod /dev/name c <major> <minor>)
```
- using udev (recommended);

To notify **udev** from the kernel module:
- create a virtual device class:
```
class_create(owner, name);
```
After calling this function the 'name' will appear under:
```
ls -l /sys/class/name
```

Then, create as device and register it with **sysfs**:
```
device_create();
```

After calling this function device name will appear under:
```
ls -l /sys/devices/virtual/<class_name>/<device_name> 
```
and
``` 
ls -l /dev/<device_name>
```

