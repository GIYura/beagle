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
bool SpiInit(int fd, uint8_t mode, uint8_t bits, uint32_t speed);

/*
* @ brief Spi read byte
* @param[in] - fd - file descriptor
* @param[in] - addr - register address
* @param[in] - value - value to store
* @param[out] - error code.
*/
uint8_t SpiReadByte(int fd, uint8_t addr, uint8_t* const value);

/*
* @ brief Spi write byte
* @param[in] - fd - file descriptor
* @param[in] - addr - register address
* @param[in] - value - value to write
* @param[out] - error code.
*/
uint8_t SpiWriteByte(int fd, uint8_t addr, uint8_t value);

#endif /* SPI_H */

