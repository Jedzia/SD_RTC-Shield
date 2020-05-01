//
// Created by Jedzia on 25.12.2019.
//

#ifndef BLINKY_DS1307_H
#define BLINKY_DS1307_H

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

//[[deprecated("Replaced by bar, which has an improved interface")]]
DEPRECATED void DS1307_i2c_init(void);

enum DS1307 {
DS1307_SECONDS =                0x00,
DS1307_MINUTES =               0x01,
DS1307_HOURS   =             0x02,
DS1307_DAY     =               0x03,
DS1307_DATE    =                0x04,
DS1307_MONTH   =             0x05,
DS1307_YEAR    =                0x06,

};


uint8_t translate_bcd_for_print(uint8_t data);

void i2c_setup(void);

void i2c_deinit(void);

uint8_t i2c_start(uint32_t i2c, uint8_t address, uint8_t mode);

uint8_t i2c_write(uint32_t i2c, uint8_t address, uint8_t reg,
                  uint8_t data);

int i2c_read(uint32_t i2c, uint8_t address, uint8_t reg);

uint8_t DS1307_Init(void);
uint8_t DS1307_IsRunning(void);
void DS1307_DoSomething(void);

#endif //BLINKY_DS1307_H
