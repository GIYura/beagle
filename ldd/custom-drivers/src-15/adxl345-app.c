#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "adxl345.h"

static Axes_t m_axes;

int main(int argc, char* argv[])
{
    int fd;
    printf("ADXL345 test application\n");
   
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        perror("failed to open the bus\n");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, ADXL345_ADDRESS) < 0)
    {
        perror("Failed to connect to the sensor\n");
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

