#### IOCTL driver

##### HOST

1. Compile application:
```
make app
```

2. Compile driver:
```
make
```

3. Transfer application to the target:
```
make copy-app
```

4. Transfer driver to the target:
```
make copy-drv
```

##### TARGET

1. Load the driver:
```
sudo insmod ioctl-driver.ko
```

2. Run the application:
```
sudo ./ioctl-app.elf /dev/ioctl-driver
```

3. Remove the driver:
```
sudo rmmod ioctl-driver
```

