#### How to compile device tree overlay

1. Run 
```
dtc -O dtb -o [file-name.dtbo] -b 0 -@ [file-name.dts]
```

2. Copy .dtbo to the target
```
scp .dtbo [target-ip]:/[file-path]
```

