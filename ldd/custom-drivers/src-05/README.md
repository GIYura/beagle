### DRIVER TEST

#### HOST

1. Compile driver code & setup
```
make
```

2. Copy driver & setup to the target
```
make copy-drv
```

#### TARGET

3. Load the driver & setup
```
sudo insmod driver.ko
sudo insmod driver-setup.ko
```

4. Make sure driver and setup loaded successfully
```
dmesg | tail -2
ls -l /dev/pcd - device file should be added
ls -l /sys/class/pcd_class - link to pcd should be added
```
5. Run
```
echo "Test string" > /dev/pcdev-0
cat /dev/pcdev-0
```

6. Unload the driver and setup
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

