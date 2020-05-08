#include <sched.h>
//
// Created by Jedzia on 25.12.2019.
//

#ifndef BLINKY_DS1307_H
#define BLINKY_DS1307_H
#ifdef    __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#define DS1307_ADDRESS        0x68  ///< I2C address for DS1307
#define DS1307_CONTROL        0x07  ///< Control register
#define DS1307_NVRAM          0x08  ///< Start of RAM registers - 56 bytes, 0x08 to 0x3f

/* Bits in control register */
#define DS1307_CONTROL_OUT            7
#define DS1307_CONTROL_SQWE           4
#define DS1307_CONTROL_RS1            1
#define DS1307_CONTROL_RS0            0


enum DS1307 {
    DS1307_SECONDS = 0x00,
    DS1307_MINUTES = 0x01,
    DS1307_HOURS = 0x02,
    DS1307_DAY = 0x03,
    DS1307_DATE = 0x04,
    DS1307_MONTH = 0x05,
    DS1307_YEAR = 0x06,
};

//[[deprecated("Replaced by bar, which has an improved interface")]]
DEPRECATED void DS1307_i2c_init(void);

uint8_t translate_bcd_for_print(uint8_t data);

void i2c_setup(void);

__unused void i2c_teardown(void);

//uint8_t i2c_start(uint32_t i2c, uint8_t address, uint8_t mode);

uint8_t DS1307_Init(void);

/**************************************************************************/
/*!
    @brief  Is the DS1307 running? Check the Clock Halt bit in register 0
    @return 1 if the RTC is running, 0 if not
*/
/**************************************************************************/
uint8_t DS1307_IsRunning(void);

void DS1307_DoSomething(void);

#ifdef    __cplusplus
}
#endif

#endif //BLINKY_DS1307_H
