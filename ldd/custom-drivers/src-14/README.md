#### DS18B20 interconnection

##### Connection

| DS18B20  | BBB   | NOTE    |
|----------|-------|---------|
| VDD      | P9.3  | +3.3 V  |
| GND      | P9.1  | GND     |
| DQ       | P8.46 | I/O     |

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

1. Load the driver:
```
sudo insmod ds18b20-driver.ko
```

2. Run application:
```
sudo ./ds18b20-app /dev/ds18b20
```

