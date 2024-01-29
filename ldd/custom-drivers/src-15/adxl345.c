#include <stdint.h>    
#include <stdio.h>
#include <unistd.h>

#include "adxl345.h"

#define BUFFER_SIZE 0x40

static uint8_t AccelRegRead(int fd, uint8_t reg, uint8_t* const value);
static uint8_t AccelRegWrite(int fd, uint8_t reg, uint8_t value);

void AccelInit(int fd)
{
    uint8_t devId = 0x00;
    uint8_t ret = 0;
    ret = AccelRegRead(fd, DEVID, &devId);
    if (ret != 0)
        printf("ERROR: 0x%02x: Failed to read DEVID\n", ret);
    else
        printf("DEV ID: 0x%02x\n", devId);

#if 0
    uint8_t bwRate = 0x00;
    if (AccelRegRead(fd, BW_RATE, &bwRate) == 0)
        printf("BW RATE: 0x%02x\n", bwRate);

    uint8_t intSrc = 0x00;
    if (AccelRegRead(fd, INT_SRC, &intSrc) == 0)
        printf("INT SRC: 0x%02x\n", intSrc);

    if (AccelRegWrite(fd, THRES_TAP, 0xff) == 0)
        printf("Write reg 0x%02x success\n", THRES_TAP);

    uint8_t thresTap = 0x00;
    if (AccelRegRead(fd, THRES_TAP, &thresTap) == 0)
        printf("THRES TAP: 0x%02x\n", thresTap);
#endif
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

