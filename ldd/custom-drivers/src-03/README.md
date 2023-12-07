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

5. Run
```
./test
```

6. Unload the driver
```
sudo rmmod driver.ko
```

**NOTE:** change device file access permission:
```
sudo chown [user:group] /path-to-file
```

