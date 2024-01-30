#ifndef ADXL345_H
#define ADXL345_H

#define ADXL345_ADDRESS 0x53

/* ADXL345 registers */
#define DEVID       0x00
#define BW_RATE     0x2c
#define POWER_CTL   0x2d
#define DATA_FORMAT 0x31
#define DATAX0      0x32

typedef struct
{
    float x;
    float y;
    float z;
} Axes_t;

/*
* @ brief Accelerometer init
* @param[in] - file descriptor
* @param[out] - none
*/
void AccelInit(int fd);

/*
* @ brief Get accelerometer vector
* @param[in] - file descriptor
* @param[in] - pointer to struct to store axes
* @param[out] - none
*/
void AccelVector(int fd, Axes_t* const axes);

/*
* @ brief Print accelerometer axes 
* @param[in] - pointer to struct
* @param[out] - none
*/
void AccelVectorPrint(const Axes_t* const axes);

#endif /* ADXL345_h */

