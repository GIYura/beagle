#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>

#include "ds18b20.h"
#include "gpio.h"

static void ds18b20_write_bit(uint8_t bit);
static uint8_t ds18b20_read_bit(void);

int ds18b20_reset(void)
{
    int ret = 0;

    //DS18B20_OUT;
    gpio_configure_dir(GPIO_DS18B20_DQ, GPIO_DIR_OUT);

    //DS18B20_LOW;
    gpio_write_value(GPIO_DS18B20_DQ, GPIO_LOW_VALUE);
    usleep(10);

    //DS18B20_HIGH;
    gpio_write_value(GPIO_DS18B20_DQ, GPIO_HIGH_VALUE);
    
    //DS18B20_IN;
    gpio_configure_dir(GPIO_DS18B20_DQ, GPIO_DIR_IN);
    usleep(45);

    //ret = DS18B20_GET_DATA;
    ret = gpio_read_value(GPIO_DS18B20_DQ);
    usleep(420);

    return ret;
}

uint8_t ds18b20_read_byte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    while (i--)
    {
        byte >>= 1;
        byte |= (ds18b20_read_bit() << 7);
    }
    return byte;
}

void ds18b20_write_byte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--)
    {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

static void ds18b20_write_bit(uint8_t bit)
{
    gpio_configure_dir(GPIO_DS18B20_DQ, GPIO_DIR_OUT);
    //DS18B20_OUT;
    
    gpio_write_value(GPIO_DS18B20_DQ, GPIO_LOW_VALUE);
    //DS18B20_LOW;
    usleep(1);

    if (bit)
        gpio_write_value(GPIO_DS18B20_DQ, GPIO_DIR_IN);
    //{
    //    DS18B20_IN;
    //}
    
    usleep(60);
    
    gpio_write_value(GPIO_DS18B20_DQ, GPIO_DIR_IN);
    //DS18B20_IN;
}

static uint8_t ds18b20_read_bit(void)
{
    uint8_t bit;

    gpio_configure_dir(GPIO_DS18B20_DQ, GPIO_DIR_OUT);
    //DS18B20_OUT;
    
    gpio_write_value(GPIO_DS18B20_DQ, GPIO_LOW_VALUE);
    //DS18B20_LOW;
    usleep(1);

    gpio_write_value(GPIO_DS18B20_DQ, GPIO_DIR_IN);
    //DS18B20_IN;
    usleep(14);

    bit = (uint8_t)gpio_read_value(GPIO_DS18B20_DQ);
    //bit = DS18B20_GET_DATA;

    usleep(45);

    return bit;
}

