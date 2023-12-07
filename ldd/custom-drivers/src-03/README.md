### DRIVER TEST

1. Copy driver & test to the target
```
make copy_drv
make copy_tst
```

2. Load the driver
```
sudo insmod driver.ko
```

3. Make sure driver loaded successfully
```
dmesg | tail -2
ls -l /dev/pcd - device file should be added
ls -l /sys/class/pcd_class - link to pcd should be added
```

4. Compile 'test'
```
gcc test.c -o test
```

5. Write data to the device file
```
echo "TEST STRING" > /path-to-device-file
```

6. Run
```
./test [device][read-count]
```

7. Unload the driver
```
sudo rmmod driver.ko
```

**NOTE:** change device file access permission:
```
sudo chown [user:group] /path-to-file
```

**NOTE:** device permissions:
```
/dev/pcdev-1 - RO
/dev/pcdev-2 - WO
/dev/pcdev-3 - RW
/dev/pcdev-4 - RW
```

![driver](https://github.com/GIYura/beagle/tree/main/ldd/custom-drivers/src-03/driver-mult-devs.png)

