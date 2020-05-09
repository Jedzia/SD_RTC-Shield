#include <sched.h>
//
// Created by Jedzia on 25.12.2019.
//

#include "DS1307.h"
//#include "clock.h"
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/assert.h>

uint8_t DS1307_IsRunning(void) {
    // Todo: there is no timeout check in i2c_transfer7, build one.
    // This should check if the DS1307 is initialized and return false when not.
    // Then the DS1307 should be initialized via DS1307_Init().
    uint8_t cmd = DS1307_SECONDS;
    uint8_t data = 42;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &data, 1);
    printf("DS1307_IsRunning: %d\n", data);
    return !(data >> 7);
}

uint8_t translate_bcd_for_print(uint8_t data) {
    cm3_assert(((data & 0xF0) >> 4) < 10);  // More significant nybble is valid
    cm3_assert((data & 0x0F) < 10);         // Less significant nybble is valid
    uint8_t dec = ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
    return dec;
}

void DS1307_DoSomething() {
    //uint8_t val1 = 41, val2 = 42, val3 = 43, val4 = 44, val5 = 45, val6 = 46, val7 = 47, val8 = 48, val9 = 49;

    uint8_t cmd = 0x55;
    uint8_t time_buf[8] = {};
    cmd = DS1307_SECONDS;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, time_buf, sizeof(time_buf));

    printf("sec: %d, min: %d , hrs: %d , dow: %d , date: %d , month: %d , year: %d\n",
            translate_bcd_for_print(time_buf[0]),
            translate_bcd_for_print(time_buf[1]),
            translate_bcd_for_print(time_buf[2]),
            translate_bcd_for_print(time_buf[3]),
            translate_bcd_for_print(time_buf[4]),
            translate_bcd_for_print(time_buf[5]),
            translate_bcd_for_print(time_buf[6])
    );
}

uint8_t DS1307_Init(void) {

    // ToDo: initialize the DS1307 to default time data here.
    // without it does not run.

    // this shit is obolete
    return 0;
//    if(i2c_start(I2C1, DS1307_ADDRESS, I2C_WRITE) != 0)
//    {
//        printf("i2c_start: Timeout\n");
//        return -1;
//    }
//
//    i2c_send_data(I2C1, DS1307_CONTROL);
//
//    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));
//    i2c_send_data(I2C1, 0);
//
//    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));
//
//    i2c_send_stop(I2C1);
//
//    return 0;
//
//    return 0;
//    uint32_t reg32 __attribute__((unused));
//
//    /* Send START condition. */
//    printf("i2c_send_start \n");
//    i2c_send_start(I2C1);
//
//    /* Waiting for START is send and switched to master mode. */
//    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
//             & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
//
//    /* Send destination address. */
//    printf("Send destination address \n");
//    i2c_send_7bit_address(I2C1, DS1307_ADDRESS << 1, I2C_WRITE);
//
//    /* Waiting for address is transferred. */
//    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
//
//    /* Cleaning ADDR condition sequence. */
//    reg32 = I2C_SR2(I2C1);
//
//    /* Sending the data. */
//    printf("Sending the data \n");
//    i2c_send_data(I2C1, DS1307_SECONDS); /* OvertemperatureShutdown register */
//    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));
//    i2c_send_data(I2C1, 42); /* MSB */
//    //while (!(I2C_SR1(i2c) & I2C_SR1_BTF));
//    //i2c_send_data(I2C1, (uint8_t)(temp_os & 0xff00)); /* LSB */
//    /* After the last byte we have to wait for TxE too. */
//    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));
//
//    /* Send STOP condition. */
//    //printf("Send STOP condition \n");
//    i2c_send_stop(I2C1);
//
//    return 0;
}

void DS1307_i2c_init(void) {
    // ToDo: only as reference from https://github.com/libopencm3/libopencm3-examples/blob/d076cb5549a5425201df50770fa4b96bd73c4568/examples/stm32/f4/stm32f429i-discovery/lcd-serial-touch/touch_i2c.c, delete after R&D
    /* Enable clocks for I2C2 and GPIO. */
    //rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    //rcc_periph_clock_enable(RCC_GPIOC);
    //rcc_periph_clock_enable(RCC_GPIOH);
    rcc_periph_clock_enable(RCC_I2C1);

    /* Set gpio and alternate functions for the SCL and SDA pins of I2C3. */
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    // ToDo: Added gpio_set_output_options, because GPIO_OTYPE_OD is needed for operation. Give feedback to libopencm3?
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO9);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C1);

    i2c_set_speed(I2C1, i2c_speed_sm_100k, I2C_CR2_FREQ_42MHZ);

    /*
     * This is our slave address - needed only if we want to receive from
     * other masters.
     */
    i2c_set_own_7bit_slave_address(I2C1, 0x32);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C1);
}

void i2c_setup(void) {

    // I2C1 is used on PB8+PB9, enable Clock for Peripherals and I2C
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);

    // PB8 - I2C1_SCL
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
    // HAL sets #define  GPIO_MODE_AF_OD  0x00000012U   /*!< Alternate Function Open Drain Mode    */ Is needed!
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8);
    // HAL sets #define GPIO_AF4_I2C1 ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8);

    // PB9 - I2C1_SDA
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO9);

    i2c_peripheral_disable(I2C1); /* disable i2c during setup */

    //    i2c_reset(I2C1);

    //    i2c_set_fast_mode(I2C1);
    //    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_42MHZ);

    // 84 is wrong and results in 50kHz SCL clock frequency. Use I2C_CR2_FREQ_42MHZ with a 84MHz .ahb_frequency
    // and .apb1_frequency = 42000000,
    i2c_set_speed(I2C1, i2c_speed_sm_100k, I2C_CR2_FREQ_42MHZ);

    //i2c_set_ccr(I2C1, 35);
    //i2c_set_trise(I2C1, 43);

    i2c_set_own_7bit_slave_address(I2C1, 0x32);

    i2c_peripheral_enable(I2C1); /* finally enable i2c */
}


__unused void i2c_teardown(void) {
    i2c_send_stop(I2C1);

    i2c_reset(I2C1);
    i2c_peripheral_disable(I2C1); /* disable i2c during setup */

}


//uint8_t i2c_start(uint32_t i2c, uint8_t address, uint8_t mode) {
//    i2c_send_start(i2c);
//
//    /* Wait for master mode selected */
//    while(!((I2C_SR1(i2c) & I2C_SR1_SB)
//            & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
//
//    i2c_send_7bit_address(i2c, address, mode);
//
//    int timeout = 20000;
//    /* Waiting for address is transferred. */
//    while(!(I2C_SR1(i2c) & I2C_SR1_ADDR)) {
//        if(timeout > 0) {
//            timeout--;
//        } else {
//            return 1;
//        }
//    }
//
//
//    /* Cleaning ADDR condition sequence. */
//    uint32_t reg32 = I2C_SR2(i2c);
//    (void) reg32; /* unused */
//
//    return 0;
//}



