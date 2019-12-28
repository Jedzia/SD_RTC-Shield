//
// Created by Jedzia on 25.12.2019.
//

#include "DS1307.h"
#include "clock.h"
#include <stdio.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/assert.h>

#define DS1307_ADDRESS        0x68  ///< I2C address for DS1307
#define DS1307_CONTROL        0x07  ///< Control register
#define DS1307_NVRAM          0x08  ///< Start of RAM registers - 56 bytes, 0x08 to 0x3f

void DS1307_DoSomething() {
    uint8_t val = i2c_read(I2C1, DS1307_ADDRESS, DS1307_CONTROL);
    printf("I2C Value: %d\n", val);
}

void i2c_setup(void) {

//      *** from HAL ***
//    /* USER CODE END I2C1_MspInit 0 */
//
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C1 GPIO Configuration
//    PB8     ------> I2C1_SCL
//    PB9     ------> I2C1_SDA
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    /* Peripheral clock enable */
//    __HAL_RCC_I2C1_CLK_ENABLE();
//    /* USER CODE BEGIN I2C1_MspInit 1 */
//

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOH);
    rcc_periph_clock_enable(RCC_I2C1);

    // was PA8 - I2C3_SCL
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
    // HAL sets #define  GPIO_MODE_AF_OD  0x00000012U   /*!< Alternate Function Open Drain Mode    */
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8);
    // HAL sets #define GPIO_AF4_I2C1 ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8);

    // was PC9 - I2C3_SDA
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO9);

    i2c_peripheral_disable(I2C1); /* disable i2c during setup */
    i2c_reset(I2C1);

    i2c_set_fast_mode(I2C1);
    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_42MHZ);
    i2c_set_ccr(I2C1, 35);
    i2c_set_trise(I2C1, 43);
    //i2c_set_speed(I2C1, 0);

    i2c_peripheral_enable(I2C1); /* finally enable i2c */

    i2c_set_own_7bit_slave_address(I2C1, 0x00);
}


void i2c_deinit(void) {
    i2c_send_stop(I2C1);

    i2c_reset(I2C1);
    i2c_peripheral_disable(I2C1); /* disable i2c during setup */

}


uint8_t i2c_start(uint32_t i2c, uint8_t address, uint8_t mode) {
    i2c_send_start(i2c);

    /* Wait for master mode selected */
    while(!((I2C_SR1(i2c) & I2C_SR1_SB)
            & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    i2c_send_7bit_address(i2c, address, mode);

    int timeout = 20000;
    /* Waiting for address is transferred. */
    while(!(I2C_SR1(i2c) & I2C_SR1_ADDR)) {
        if(timeout > 0) {
            timeout--;
        } else {
            return 1;
        }
    }


    /* Cleaning ADDR condition sequence. */
    uint32_t reg32 = I2C_SR2(i2c);
    (void) reg32; /* unused */

    return 0;
}


uint8_t i2c_write(uint32_t i2c, uint8_t address, uint8_t reg, uint8_t data) {
    i2c_start(i2c, address, I2C_WRITE);

    i2c_send_data(i2c, reg);

    while(!(I2C_SR1(i2c) & (I2C_SR1_BTF)));
    i2c_send_data(i2c, data);

    while(!(I2C_SR1(i2c) & (I2C_SR1_BTF)));

    i2c_send_stop(i2c);

    return 0;
}

int i2c_read(uint32_t i2c, uint8_t address, uint8_t reg) {
    uint32_t timeout = 20000;
    while((I2C_SR2(i2c) & I2C_SR2_BUSY)); // {
//		if (timeout > 0) {
//			timeout--;
//		} else {
//			return -1;
//		}
//	}

    if(i2c_start(i2c, address, I2C_WRITE)) {
        return 0;
    }
    i2c_send_data(i2c, reg);

    timeout = 20000;
    while(!(I2C_SR1(i2c) & (I2C_SR1_BTF))) {
        if(timeout > 0) {
            timeout--;
        } else {
            return -1;
        }
    }

    i2c_start(i2c, address, I2C_READ);

    i2c_send_stop(i2c);

    while(!(I2C_SR1(i2c) & I2C_SR1_RxNE));

    int result = (int) i2c_get_data(i2c);

    I2C_SR1(i2c) &= ~I2C_SR1_AF;
    msleep(50);
    i2c_send_stop(i2c);

    return result;
}

