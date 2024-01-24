#### DS18B20 interconnection

##### Connection

| DS18B20  | BBB   | NOTE    |
|----------|-------|---------|
| VDD      | P9.3  | +3.3 V  |
| GND      | P9.1  | GND     |
| DQ       | P8.46 | I/O     |

NOTE:
```
Pin P8.46 is a physical pin on BBB, it is named GPIO2_7.
To calculate gpio number: gpio2_7 = 2 * 32 + 7 = 71
```

#### HOST

1. Compile driver:
```
make
```

2. Compile application:
```
make app
```

3. Transfer driver to the target:
```
make copy-drv
```

4. Transfer application to the target:
```
make copy-app
```

#### TARGET

1. To find out driver parameters:
```
modinfo -F parm <file-name.ko>
```

2. Load the driver:
```
sudo insmod ds18b20-driver.ko <gpio pin number>
```

3. Run application:
```
sudo ./ds18b20-app /dev/ds18b20
```

4. To change gpio pin number:
```
sudo sh -c "echo [pin-number] > /sys/module/ds18b20_driver/parameters/cb_gpio_pin"
``` 

**NOTE:** pin-number should be within range 70 - 77.

