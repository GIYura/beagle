#### Accelerometer (ADXL345)

##### Connection

1. Pinout

| ADXL345  | BBB     | Notes      |
|----------|---------|------------|
|  GND     |  P9.1   |  GND       |
|  VCC     |  P9.3   |  3.3V      |
|  CS      |  P9.3   |  3.3V      |
|  SCL     |  P9.17  |  I2C1_SCL  |
|  SDA     |  P9.18  |  I2C1_SDA  |

##### HOST

1. Compile application:
```
make app
```

2. Transfer application to the target:
```
make copy-app
```

3. Compile driver:
```
make
```

4. Transfer driver to the target:
```
make copy-drv
```

##### TARGET

1. Load the driver:
```
sudo insmod adxl345-driver.ko
```

2. Make sure the driver loaded:
```
lsmod

ls -l /dev/adxl345-i2c
```

3. Run application:
```
sudo ./adxl345-app.elf /dev/adxl345-i2c
```

4. Unload the driver:
```
sudo rmmod adxl345-driver
```

##### Troubleshooting

1. Edit /boot/uEnv.txt:

- enable overlays:
```
enable_uboot_overlays=1
```

- enable I2C bus:
```
uboot_overlay_addr6=BB-I2C1-00A0.dtbo
```

2. Install I2C tool:
```
sudo apt-get install i2c-tools
```

3. Detect device on the bus:
```
i2sdetect -y -r 1
```

**NOTE:** 
last param is bus number [1 - 2].

The output should be 0x53 - adxl345 address.

