# This is local makefile for kbuild system
# -m - compile dynamicaly loadable kernel module
obj-m := gpio-sysfs.o 
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
#KERN_DIR=/lib/modules/4.14.108+/source
KERN_DIR=/lib/modules/5.4.106+/source
HOST_IP=192.168.7.2
DRV_PATH=/home/jura/driver/09

# make goes to the path /lib/modules/... triggers top level makefile, 
# which gets the value of M and M points to the current directory.
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) clean

help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN_DIR) M=$(PWD) help

copy_drv:
	scp *.ko $(USER)@$(HOST_IP):$(DRV_PATH)

