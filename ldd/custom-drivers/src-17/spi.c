#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

static unsigned char m_send[2];
static unsigned char m_receive[2];

/*
struct spi_ioc_transfer = {
    .tx_buf = (unsigned long)m_send,
    .rx_buf = (unsigned long)m_receive,
    .len = 2,
};
*/

static struct spi_ioc_transfer m_transfer;

bool SpiInit(int fd, unsigned int mode, unsigned int bits, unsigned int speed)
{
    assert(fd > 0);
    assert(mode >= 0 && mode <= 3);
    assert(bits == 8);
    assert(speed == 5000000);

    m_transfer.tx_buf = (unsigned long)m_send;
    m_transfer.rx_buf = (unsigned long)m_receive;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        perror("SPI: Failed to set SPI mode\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        perror("SPI: Failed to get SPI mode\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        perror("SPI: Failed to set bits per word\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
    {
        perror("SPI: Failed to get bits per word\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        perror("SPI: Failed to set max speed Hz\n");
        return false;
    }

    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
    {
        perror("SPI: Failed to get max speed Hz\n");
        return false;
    }
    return true;
}

unsigned int SpiRead(int fd, unsigned char addr, unsigned char* buffer, unsigned char size)
{
    m_send[0] = addr;

    if (ioctl(fd, SPI_IOC_MESSAGE(size), &m_transfer) < 0)
    {
        perror("SPI: Failed to read message\n");
        return -1;
    }

    buffer[0] = m_receive[1];

    return 0;
}

