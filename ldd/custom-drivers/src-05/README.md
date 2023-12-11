### DRIVER TEST

1. Copy driver & setup to the target
```
make copy_drv
```

2. Load the driver and its setup
```
sudo insmod driver.ko
sudo insmod driver-setup.ko
```

3. Make sure driver and setup loaded successfully
```
dmesg | tail -2
ls -l /dev/pcd - device file should be added
ls -l /sys/class/pcd_class - link to pcd should be added
```
4. Run
```
echo "Test string" > /dev/pcdev-0
cat /dev/pcdev-0
```

5. Unload the driver and setup
```
sudo rmmod driver.ko
sudo rmmod driver-setup.ko
```

**NOTE:** change device file access permission:
```
sudo chown [user:group] /path-to-file
```

**NOTE:** access permissions:
- pcdev-0 - RD/WR;
- pcdev-1 - RD/WR;
- pcdev-2 - RD;
- pcdev-3 - WR;

