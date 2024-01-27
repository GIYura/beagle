#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define DEVID       0x00
#define BUFFER_SIZE 40

int main()
{
   int file;
   printf("Starting the ADXL345 test application\n");
   
    if ((file = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        perror("failed to open the bus\n");
        return 1;
    }

    if (ioctl(file, I2C_SLAVE, 0x53) < 0)
    {
        perror("Failed to connect to the sensor\n");
        return 1;
    }
    
    char writeBuffer[1] = {0x00};
    if (write(file, writeBuffer, 1) != 1)
    {
        perror("Failed to reset the read address\n");
        return 1;
    }
    
    char readBuffer[BUFFER_SIZE];
    if (read(file, readBuffer, BUFFER_SIZE) != BUFFER_SIZE)
    {
        perror("Failed to read in the buffer\n");
        return 1;
    }
    printf("ADXL345 ID: 0x%02x\n", readBuffer[DEVID]);
    
    close(file);

    return 0;
}

