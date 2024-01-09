#### DS18B20 (default pin usage)

##### Connection

1. Pinout

|   BBB       | GPIO number  |   DS18b20   | Notes        |
|-------------|--------------|-------------|--------------|
| 3,4         | -            |  VDD        | +3.3 V       |
| 1,2         | -            |  GND        | GND          |
| P9.12       | gpio1.28     |  DQ         | I/O          |

##### Settings

1. Edit file /boot/uEnv.txt
```
sudo vim /boot/uEnv.txt
```

2. Edit line
```
uboot_overlay_addr4=BB-W1-P9.12-00A0.dtbo
```

3. Run
```
sudo reboot
```

**NOTE:**

ds18b20 .dtbo file location:
```
ls -l /lib/firmware/BB-W1-P9.12-00A0.dtbo
```
ds18b20 .dts file location:
```
ls -l /opt/source/bb.org-overlays/src/arm/BB-W1-P9.12-00A0.dts
```

#### DS18B20 (custom pin usage)

##### Connection

1. Pinout

|   BBB       | GPIO number  |   DS18b20   | Notes        |
|-------------|--------------|-------------|--------------|
| 3,4         | -            |  VDD        | +3.3 V       |
| 1,2         | -            |  GND        | GND          |
| P8.46       | gpio2.7      |  DQ         | I/O          |

##### Settings

**HOST**
1. Create .dts (include pin number in the name);
2. Compile .dts into .dtbo;
3. Transfer .dtbo to the target;

**TARGET**
1. Move .dtbo into /lib/firmware;
2. Modify /boot/uEnv.txt;

**NOTE:** the following lines should be uncommented:
```
disable_uboot_overlay_video=1
disable_uboot_overlay_audio=1
```
line 
```
#enable_uboot_cape_universal=1
```
should be commented.

3. Save /boot/uEnv.txt;
4. Reboot the target;

##### TEST

1. Transfer file /scripts/temp to the target
```
scp /scripts/temp <target-IP>
```

2. Run
```
./temp
```
