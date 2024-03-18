#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl-driver.h"

int main(int argc, char* argv[])
{
    int value = 0;
    struct test tst = { 4, "Jura" };

    if (argc < 2)
    {
        printf("Invalid number of arguments\n");
        printf("Usage: sudo ./ioctl-app.elf /dev/ioctl-driver\n");
        return -1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        printf("Failed to open file!\n");
        return -1;
    }

    ioctl(fd, RD_VALUE, &value);
    printf("Default value: %d\n", value);

    value = 123;

    ioctl(fd, WR_VALUE, &value);
    ioctl(fd, RD_VALUE, &value);

    printf("New value: %d\n", value);

    ioctl(fd, GREETER, &tst);

    fd = close(fd);
    if (fd < 0)
    {
        printf("Failed to close file!\n");
    }   

    return 0;
}

