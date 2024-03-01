#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/spi/spidev.h>

#include "adxl345.h"

static Axes_t m_axes;
static const char *m_device = "/dev/spidev0.0";

int main(int argc, char* argv[])
{
   int fd;
   printf("ADXL345 test application\n");
   
    if ((fd = open(m_device, O_RDWR)) < 0)
    {
        perror("failed to open the bus\n");
        return 1;
    }
   
    AccelInit(fd);

    while (1)
    {
        AccelVector(fd, &m_axes); 
        AccelVectorPrint(&m_axes);

        sleep (1);
    }

    close(fd);

    return 0;
}

