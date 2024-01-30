#include <stdint.h>    
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "adxl345.h"

#define BUFFER_SIZE     0x40
#define GRAVITY         9.81 /* m/s^2 */

static uint8_t AccelRegRead(int fd, uint8_t reg, uint8_t* const value);
static uint8_t AccelRegReadBlock(int fd, uint8_t reg, uint8_t* buff, uint8_t size);
static uint8_t AccelRegWrite(int fd, uint8_t reg, uint8_t value);

void AccelInit(int fd)
{
    uint8_t devId = 0x00;
    uint8_t ret = 0;
    ret = AccelRegRead(fd, DEVID, &devId);
    if (ret != 0)
    {
        printf("ERROR: 0x%02x: Failed to read DEVID\n", ret);
        return;
    }
    else
        printf("DEV ID: 0x%02x\n", devId);

    /* 
    - output data rate - 200 Hz;
    - normal operation
    */
    AccelRegWrite(fd, BW_RATE, 0x0b);
    
    /*
    - range - +- 16g;
    - full resolution;
    - right justified
    */
    AccelRegWrite(fd, DATA_FORMAT, 0x0b);

    /*
    - measure mode on;
    */
    AccelRegWrite(fd, POWER_CTL, 0x08);
}

void AccelVector(int fd, Axes_t* const axes)
{
    uint8_t buff[6];
    AccelRegReadBlock(fd, DATAX0, buff, sizeof(buff));

    int x = buff[0] | (buff[1] << 8);
    if (x & (1 << 15))
        x = x - (1 << 16);
    float x1 = (x / pow(2, 15)) * 16 * GRAVITY;

    int y = buff[2] | (buff[3] << 8);
    if (y & (1 << 15))
        y = y - (1 << 16);
    float y1 = (y / pow(2, 15)) * 16 * GRAVITY;

    int z = buff[4] | (buff[5] << 8);
    if (z & (1 << 15))
        z = z - (1 << 16);
    float z1 = (z / pow(2, 15)) * 16 * GRAVITY;
    
    axes->x = x1;
    axes->y = y1;
    axes->z = z1; 
}

void AccelVectorPrint(const Axes_t* const axes)
{
    printf("x = %.3f m/s^2\n", axes->x);
    printf("y = %.3f m/s^2\n", axes->y);
    printf("z = %.3f m/s^2\n", axes->z);
    printf ("------------------\n");
}

static uint8_t AccelRegRead(int fd, uint8_t reg, uint8_t* const value)
{
    char writeBuffer[1] = { 0x00 };
    char readBuffer[BUFFER_SIZE];
    
    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return 1;
    }

    if ((reg >= 0x01 && reg <= 0x1C) || reg > 0x39)
    {
        printf("Invalid register\n");
        return 2;
    }

    if (write(fd, writeBuffer, 1) != 1)
    {
        printf("Failed to reset read address\n");
        return 3;
    }

    if (read(fd, readBuffer, BUFFER_SIZE) != BUFFER_SIZE)
    {
        printf("Failed to read buffer\n");
        return 4;
    }

    *value = readBuffer[reg];

    return 0;
}

static uint8_t AccelRegReadBlock(int fd, uint8_t reg, uint8_t* buff, uint8_t size)
{
    char writeBuffer[1] = { reg };
    
    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return 1;
    }

    if ((reg >= 0x01 && reg <= 0x1C) || reg > 0x39)
    {
        printf("Invalid register\n");
        return 2;
    }

    if (write(fd, writeBuffer, 1) != 1)
    {
        printf("Failed to reset read address\n");
        return 3;
    }

    if (read(fd, buff, size) != size)
    {
        printf("Failed to read buffer\n");
        return 4;
    }

    return 0;
}

static uint8_t AccelRegWrite(int fd, uint8_t reg, uint8_t value)
{
    char writeBuffer[2] = { 0x00 };

    if (fd < 0)
    {
        printf("Invalid file descriptor\n");
        return 1;
    }

    if ((reg >= 0x01 && reg <= 0x1C) || reg > 0x39)
    {
        printf("Invalid register\n");
        return 2;
    }

    writeBuffer[0] = reg;
    writeBuffer[1] = value;

    if (write(fd, writeBuffer, 2) != 2)
    {
        printf("Failed to write\n");
        return 3;
    }

    return 0;
}   

