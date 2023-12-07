### DRIVER TEST

1. Copy driver to the target
```
make copy
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

4. Run
```
echo hello target > /dev/pcd - write 'hello' string to driver
cat /dev/pcd - read the string from driver
```

5. Unload the driver
```
sudo rmmod driver.ko
```

