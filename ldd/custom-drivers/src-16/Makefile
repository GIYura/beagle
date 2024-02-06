# This is local makefile for kbuild system
# -m - compile dynamicaly loadable kernel module
obj-m := adxl345-driver.o 
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/lib/modules/5.10.168/source
HOST_IP=192.168.7.2
DRV_PATH=~/driver/16

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

app: adxl345-app.o adxl345-app.elf

adxl345-app.o: adxl345-app.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

adxl345-app.elf: adxl345-app.o
	$(CROSS_COMPILE)gcc -o $@ $^

