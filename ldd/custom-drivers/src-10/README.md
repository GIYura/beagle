#### LCD connection

|   BBB       | GPIO number  |   LCD   | Notes        |
|-------------|--------------|---------|--------------|
| P8.7        | gpio2.2      |  4(RS)  | data/command |
| P8.46       | gpio2.7      |  5(RW)  | read/write   |
| P8.43       | gpio2.8      |  6(EN)  | enable       |
| P8.44       | gpio2.9      |  11(D4) | data line 4  |
| P8.41       | gpio2.10     |  12(D5) | data line 5  |
| P8.42       | gpio2.11     |  13(D6) | data line 6  |
| P8.39       | gpio2.12     |  14(D7) | data line 7  |
| P9.1 (GND)  |              |  BKLTA  | backlight +  |
| P9.7 (+5 V) |              |  BKLTK  | backlight -  |
| P9.1 (GND)  |              |  1 VSS  | ground       |
| P9.7 (+5 V) |              |  2 VCC  | +5 V         |


![app-struct]()

