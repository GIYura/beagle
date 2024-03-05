#include <assert.h>
#include <stdint.h>    
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "adxl345.h"
#include "spi.h"

#define ADXL345_ID      0xE5

#define GRAVITY         9.81    /* free fall acceleration in m/s^2 */

static uint8_t AccelRegRead(int fd, uint8_t reg, uint8_t* const value);
static uint8_t AccelRegReadBlock(int fd, uint8_t reg, uint8_t* const buff, uint8_t size);
static uint8_t AccelRegWrite(int fd, uint8_t reg, uint8_t value);
static bool AccelRegIsValid(uint8_t reg);

void AccelInit(int fd)
{
    uint8_t devId = 0;
    uint8_t ret = 0;
    
    uint8_t spiMode = 3; 
    uint8_t spiBits = 8;
    uint32_t spiSpeed = 5000000; /* Hz */
    
    if (!SpiInit(fd, spiMode, spiBits, spiSpeed))
    {
        printf("ERROR: Failed to init SPI\n");
        assert(0);
    }

    ret = AccelRegRead(fd, DEVID, &devId);
    if (ret != 0)
    {
        printf("ERROR: 0x%02x: Failed to read DEVID\n", ret);
        assert(0);
    }
    else
    {
        if (devId != ADXL345_ID)
        {
            printf("ERROR: Invalid DEVID\n");
            assert(0);
        }
    
        printf("DEV ID: 0x%02x\n", devId);
    }

    /* 
    - output data rate - 200 Hz;
    - normal operation
    */
    ret = AccelRegWrite(fd, BW_RATE, 0x0b);
    if (ret != 0)
    {
        printf("ERROR: 0x%02x: Failed to write\n", ret);
        assert(0);
    }
    
    /*
    - range - +- 16g;
    - full resolution;
    - right justified
    */
    ret = AccelRegWrite(fd, DATA_FORMAT, 0x0b);
    if (ret != 0)
    {
        printf("ERROR: 0x%02x: Failed to write\n", ret);
        assert(0);
    }
    /*
    - measure mode on;
    */
    ret = AccelRegWrite(fd, POWER_CTL, 0x08);
    if (ret != 0)
    {
        printf("ERROR: 0x%02x: Failed to write\n", ret);
        assert(0);
    }
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
    if (fd < 0)
    {
        printf("ERROR: Invalid file descriptor\n");
        return 1;
    }

    if (!AccelRegIsValid(reg))
    {
        printf("ERROR: Invalid register\n");
        return 2;
    }

    if (SpiReadByte(fd, reg, value) != 0)
    {
        printf("ERROR: Failed to read SPI\n");
        return 3;
    }

    return 0;
}

static uint8_t AccelRegReadBlock(int fd, uint8_t reg, uint8_t* const buff, uint8_t size)
{
    if (fd < 0)
    {
        printf("ERROR: Invalid file descriptor\n");
        return 1;
    }

    if (!AccelRegIsValid(reg))
    {
        printf("ERROR: Invalid register\n");
        return 2;
    }

    for (uint8_t i = 0; i < size; i++)
    {
        AccelRegRead(fd, reg, &buff[i]);
        reg++;
    }

    return 0;
}

static uint8_t AccelRegWrite(int fd, uint8_t reg, uint8_t value)
{
    if (fd < 0)
    {
        printf("ERROR: Invalid file descriptor\n");
        return 1;
    }

    if (!AccelRegIsValid(reg))
    {
        printf("ERROR: Invalid register\n");
        return 2;
    }

    if (SpiWriteByte(fd, reg, value) != 0)
    {
        printf("ERROR: Failed to write SPI\n");
        return 3;
    }

    return 0;
}   

static bool AccelRegIsValid(uint8_t reg)
{
    return ((reg < 0x01 || reg > 0x1C) && reg < 0x39) ? true : false;
}

