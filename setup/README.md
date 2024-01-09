#### Serial connection

| TTL converter | BBB (J1 header)  |
|---------------|------------------|
|      GND      |     GND (J1:1)   |
|      TX       |     RX  (J1:4)   |
|      RX       |     TX  (J1:5)   |

NOTE: 

- default baudrate: 115200
- format: 8N1
- user name: **debian**
- password: **temppwd**

5. To change password run the following command:
``` 
passwd
```
Follow instructions
                 
6. To add the user
```
sudo adduser <user-name>
```
- Enter **new** password and follow instructions.

7. To add new user to sudo group
```
sudo usermod -a -G <group-to-add> <user-name>
```
Example: sudo usermod -a -G sudo [user-name]

8. Set timezone
```
timedatectl list-timezones
sudo timedatectl set-timezone <name-of-time-zone>
```

