#### On-board LEDs

##### Connection

1. Pinout

|   BBB LED   | GPIO number  |
|-------------|--------------|
| USR0        | GPIO1_21     |
| USR1        | GPIO1_22     |
| USR2        | GPIO1_23     |
| USR3        | GPIO1_24     |

##### Settings (HOST)

1. Modify (create) .dtsi file, located in /dts directory of the current project;

2. Copy .dtsi file to source:
```
cp <file.dtsi> /linux/arch/arm/boot/dts
```

3. Edit main .dts file, run:
```
vim am335x-boneblack.dts
```

4. Include desired .dtsi file;

5. Edit am335x-bone-common.dtsi. Comment leds section.
NOTE: this section might be used by come other driver.

6. Compile main .dts into .dtb
```
make ARHC=arm CROSS_COMPILE=arm-linux-gnueabihf- am335x-boneblack.dtb
```

7. Trasfer .dtb into target
```
scp .dtb <USER>@<TARGET_IP>:/location
```

##### Settings (TARGET)

1. Edit file /boot/uEnv.txt
```
sudo vim /boot/uEnv.txt
```

2. Edit line (disable overlays)
```
#uboot_overlay_overlays=1
```

3. Copy .dtb into boot:
````
sudo cp .dtb /boot/dts/<core-version>
```

4. Run
```
sudo reboot
```

##### TEST

1. Compile driver
```
make 
```

2. Trasfer driver to the target:
```
make copy-drv
```

3. Load the driver:
```
sudo insmod .ko
```

4. Transfer file /scripts/led to the target
```
scp /scripts/temp <USER>@<TARGET-IP>:/location
```

5. Run
```
./led
```

