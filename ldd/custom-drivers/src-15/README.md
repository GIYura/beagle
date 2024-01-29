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
make
```

2. Transfer application to the target:
```
make copy-app
```

##### TARGET

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
NOTE: last param is bus number [1 - 2].

The output should be 0x53 - adxl345 address.

4. Run application:
```
./adxl345-app
```

