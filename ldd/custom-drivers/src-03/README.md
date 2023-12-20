### DRIVER TEST

#### HOST

1. Compile driver code
```
make
```

2. Copy driver & test to the target
```
make copy-drv
make copy-tst
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

5. Compile 'test'
```
gcc test.c -o test
```

6. Write data to the device file
```
echo "TEST STRING" > /path-to-device-file
```

7. Run
```
./test [device][read-count]
```

8. Unload the driver
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

Relation between device and driver
![driver-1](https://github.com/GIYura/beagle/blob/main/ldd/custom-drivers/src-03/driver-mult-devs.png)

Access to device from application
![driver-2](https://github.com/GIYura/beagle/blob/main/ldd/custom-drivers/src-03/drv.png)

