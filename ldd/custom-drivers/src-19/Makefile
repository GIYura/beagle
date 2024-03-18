# This is local makefile for kbuild system
# -m - compile dynamicaly loadable kernel module
obj-m := ioctl-driver.o 
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN_DIR=/lib/modules/5.10.168/source
HOST_IP=192.168.7.2
DRV_PATH=~/driver/19

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

app: ioctl-app.o ioctl-app.elf

ioctl-app.o: ioctl-app.c
	$(CROSS_COMPILE)gcc -c -o $@ $^

ioctl-app.elf: ioctl-app.o
	$(CROSS_COMPILE)gcc -o $@ $^

