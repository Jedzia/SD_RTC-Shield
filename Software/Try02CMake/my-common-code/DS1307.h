//
// Created by Jedzia on 25.12.2019.
//

#ifndef BLINKY_DS1307_H
#define BLINKY_DS1307_H

#include <stdint.h>

void i2c_setup(void);

void i2c_deinit(void);

uint8_t i2c_start(uint32_t i2c, uint8_t address, uint8_t mode);

uint8_t i2c_write(uint32_t i2c, uint8_t address, uint8_t reg,
                  uint8_t data);

int i2c_read(uint32_t i2c, uint8_t address, uint8_t reg);

#endif //BLINKY_DS1307_H
