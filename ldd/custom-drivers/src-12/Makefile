# This is local makefile for kbuild system
# -m - compile dynamicaly loadable kernel module
obj-m := gpio-sysfs.o 
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/lib/modules/5.10.168/source
HOST_IP=192.168.7.2
DRV_PATH=~/driver/12
DTB_PATH=~/programming/beagle/ldd/source/linux-bbb-5.10.168-ti-r72/linux/arch/arm/boot/dts

# make goes to the path /lib/modules/... triggers top level makefile, 
# which gets the value of M and M points to the current directory.
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean
	rm -rf *.elf

help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help

copy-drv:
	scp *.ko $(USER)@$(HOST_IP):$(DRV_PATH)

copy-app:
	scp *.elf $(USER)@$(HOST_IP):$(DRV_PATH)

copy-dtb:
	scp $(DTB_PATH)/am335x-boneblack.dtb $(USER)@$(HOST_IP):~/driver

app: ds18b20-app.o ds18b20.o gpio.o ds18b20-app.elf

ds18b20-app.o: ds18b20-app.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

ds18b20.o: ds18b20.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

gpio.o: gpio.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

ds18b20-app.elf: ds18b20-app.o ds18b20.o gpio.o
	$(CROSS_COMPILE)gcc -o $@ $^

