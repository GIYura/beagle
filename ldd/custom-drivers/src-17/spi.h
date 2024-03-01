#ifndef SPI_H
#define SPI_H

#include <stdbool.h>

/*
* @ brief Spi bus init
* @param[in] - fd - file descriptor
* @param[in] - mode (0 - 3)
* @param[in] - bits (5 - 8)
* @param[in] - speed (0 - 5000000) in Hz
* @param[out] - true - init success; false - init failed.
*/
bool SpiInit(int fd, unsigned int mode, unsigned int bits, unsigned int speed);

unsigned int  SpiRead(int fd, unsigned char addr, unsigned char* buffer, unsigned char size);

#endif /* SPI_H */


