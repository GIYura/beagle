#ifndef DS18B20_H
#define DS18B20_H

#define GPIO_DS18B20_DQ "gpio1.28"  /* data pin of ds18b20 */

int ds18b20_reset(void);
uint8_t ds18b20_read_byte(void);
void ds18b20_write_byte(uint8_t byte);

#endif /* DS18B20 */

