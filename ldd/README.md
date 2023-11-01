#### Kernel compilation steps

1. Remove all the temporary folder, object files, images generated during the previous build. 
This step also deletes the .config file if created previously.
```
make ARCH=arm distclean
```

2. Create a **.config** file by using default config file given by the vendor.
```
make ARCH=arm bb.org_defconfig
```

3. This step is optional. Run this command only if you want to change some kernel settings before compilation.
```
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```

4. Kernel source code compilation. This stage creates a kernel image **uImage** also all the device tree source files will be compiled, 
and dtbs will be generated.
```
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- uImage dtbs LOADADDR=0x80008000 -j4
```

5. This step builds and generates in-tree loadable(M) kernel modules(.ko).
```
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  modules  -j4
```

6. This step installs all the generated **.ko** files in the default path of the computer **/lib/modules/<kernel_ver>**
```
sudo make ARCH=arm  modules_install
```

#### Reference

Source code:
[code](https://github.com/niekiran/linux-device-driver-1.git)

Presentation:
[ppt](https://1drv.ms/b/s!Akhv68fruLVPunnvLBoIL0-Gu-et?e=cCnfvr)

