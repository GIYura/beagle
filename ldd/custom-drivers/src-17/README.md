#### Accelerometer (ADXL345)

##### Connection

1. Pinout

| ADXL345  | BBB     | Notes      |
|----------|---------|------------|
|  GND     |  P9.1   |  GND       |
|  VCC     |  P9.3   |  3.3V      |
|  CS      |  P9.17  |  spi0_cs0  |
|  SCL     |  P9.22  |  spi0_sclk |
|  SDA     |  P9.18  |  spi0_d1   |
|  SDO     |  P9.21  |  spi0_d0   |

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

- enable SPI0 bus:
```
uboot_overlay_addr6=BB-SPIDEV0-00A0.dtbo
```

2. Run application:
```
./adxl345-app
```

