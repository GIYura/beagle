#### STEPS

1. Compile device tree overlay
```
dtc [in file type][out file type][out file name][in file name] 
dtc -@ -I dts -O dtb -o PCDEV0.dtbo PCDEV.dtbs
```

2. Update

Move .dtbo into root file system
```
scp .dtbo host@ip:/lib/firmware
```

3. U-boot

Instruct u-boot to pick this .dtbo and apply it during system startup.

NOTE: while u-boot compilation the following error might occurred:
ERROR: "multiple definition of yylloc".

To fix this do the following:
1. Edit files 
```
/scripts/dtc/dtc-lexer-lex.c 
/dtc-lexer.lex.c_shipped
```
make sure 
```
extern YYLTYPE yylloc;
```  

