### DRIVER TEST

#### HOST

1. Compile driver code
```
make
```

2. Copy driver to the target
```
make copy-drv
```

3. Add dtsi file to the source
```
cd /linux-bbb-5.4/linux/arch/arm/boot/dts
touch am335x-boneblack-ldd.dtsi
```

4. Include am335x-boneblack-ldd.dtsi into am335x-boneblack.dts
```
#include "am335x-boneblack-ldd.dtsi"
```

5. Compile device tree
```
sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack.dtb
```

6. Copy am335x-boneblack.dtb to the target
```
make copy-dtb
```

#### TARGET

7. Load the driver
```
sudo insmod driver.ko
```

8. Make sure driver and setup loaded successfully
```
dmesg | tail -2
ls -l /dev/pcd - device file should be added
ls -l /sys/class/pcd_class - link to pcd should be added
```

9. Apply new am335x-boneblack.dtb
```
sudo ./copy-dtb
```

10. Run
```
echo "Test string" > /dev/pcdev-0
cat /dev/pcdev-0
```

11. Unload the driver and setup
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

