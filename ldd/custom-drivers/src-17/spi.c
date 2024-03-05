#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

#define BUFF_SIZE   2

static unsigned char m_send[BUFF_SIZE];
static unsigned char m_receive[BUFF_SIZE];
static struct spi_ioc_transfer m_transfer;

bool SpiInit(int fd, uint8_t mode, uint8_t bits, uint32_t speed)
{
    assert(fd > 0);
    assert(mode >= 0 && mode <= 3);
    assert(bits == 8);
    assert(speed == 5000000);

    m_transfer.tx_buf = (unsigned long)m_send;
    m_transfer.rx_buf = (unsigned long)m_receive;
    m_transfer.len = BUFF_SIZE;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        printf("ERROR SPI: Failed to set SPI mode\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        printf("ERROR SPI: Failed to get SPI mode\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        printf("ERROR SPI: Failed to set bits per word\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
    {
        printf("ERROR SPI: Failed to get bits per word\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("ERROR SPI: Failed to set max speed Hz\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("ERROR SPI: Failed to get max speed Hz\n");
        return false;
    }
    return true;
}

uint8_t SpiReadByte(int fd, uint8_t addr, uint8_t* const buffer)
{
    m_send[0] = (0x80 | addr);

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &m_transfer) < 0)
    {
        printf("ERROR SPI: Failed to read message\n");
        return 1;
    }

    buffer[0] = m_receive[1];

    return 0;
}

uint8_t SpiWriteByte(int fd, uint8_t addr, uint8_t value)
{
    m_send[0] = (0x00 | addr);
    m_send[1] = value;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &m_transfer) < 0)
    { 
        printf("ERROR SPI: Failed to read message\n");
        return 1;
    }
    return 0;
}

