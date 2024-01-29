#ifndef ADXL345_H
#define ADXL345_H

#define ADXL345_ADDRESS 0x53

/* ADXL345 registers */
#define DEVID       0x00
#define THRES_TAP   0x1d

#define BW_RATE     0x2c
#define INT_SRC     0x30
#define DATA_FORMAT 0x31

void AccelInit(int fd);

#endif /* ADXL345_h */

