//
// Created by Jedzia on 25.12.2019.
//

#include "DS1307.h"
#include "clock.h"
#include <stdio.h>
#include <assert.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/assert.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#define DS1307_ADDRESS        0x68  ///< I2C address for DS1307
#define DS1307_CONTROL        0x07  ///< Control register
#define DS1307_NVRAM          0x08  ///< Start of RAM registers - 56 bytes, 0x08 to 0x3f

/* Registers location */
#define DS1307_SECONDS                0x00
#define DS1307_MINUTES                0x01
#define DS1307_HOURS                0x02
#define DS1307_DAY                    0x03
#define DS1307_DATE                    0x04
#define DS1307_MONTH                0x05
#define DS1307_YEAR                    0x06
#define DS1307_CONTROL                0x07

/* Bits in control register */
#define DS1307_CONTROL_OUT            7
#define DS1307_CONTROL_SQWE            4
#define DS1307_CONTROL_RS1            1
#define DS1307_CONTROL_RS0            0

/**************************************************************************/
/*!
    @brief  Is the DS1307 running? Check the Clock Halt bit in register 0
    @return 1 if the RTC is running, 0 if not
*/
/**************************************************************************/

uint8_t DS1307_IsRunning(void) {
    uint8_t ss = i2c_read(I2C1, DS1307_ADDRESS, DS1307_SECONDS);;
    return !(ss >> 7);
}

uint8_t translate_bcd_for_print(uint8_t data) {
    assert(((data & 0xF0) >> 4) < 10);  // More significant nybble is valid
    assert((data & 0x0F) < 10);         // Less significant nybble is valid
    uint8_t dec = ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
    return dec;
}

void DS1307_DoSomething() {
    //i2c_write(I2C1, DS1307_ADDRESS, 0, 0);

    uint8_t val1 = 41, val2 = 42, val3 = 43, val4 = 44, val5 = 45, val6 = 46, val7 = 47, val8 = 48, val9 = 49;

    /*if(i2c_start(I2C1, DS1307_ADDRESS, I2C_WRITE) != 0)
    {
        printf("i2c_start: Timeout -1\n");
        return;
    }

    i2c_send_data(I2C1, DS1307_CONTROL);

    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));
    i2c_send_data(I2C1, 0);

    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));

    i2c_send_stop(I2C1);

    printf("i2c_start: return -1\n");
    return;*/



   /* uint8_t cmd = ACC_CTRL_REG1_A;
    uint8_t data;
    i2c_transfer7(I2C1, I2C_ACC_ADDR, &cmd, 1, &data, 1);
    cmd = ACC_CTRL_REG4_A;
    i2c_transfer7(I2C1, I2C_ACC_ADDR, &cmd, 1, &data, 1);
    int16_t acc_x;

    while (1) {

        cmd = ACC_STATUS;
        i2c_transfer7(I2C1, I2C_ACC_ADDR, &cmd, 1, &data, 1);
        cmd = ACC_OUT_X_L_A;
        i2c_transfer7(I2C1, I2C_ACC_ADDR, &cmd, 1, &data, 1);
        acc_x = data;
        cmd = ACC_OUT_X_H_A;
        i2c_transfer7(I2C1, I2C_ACC_ADDR, &cmd, 1, &data, 1);
        acc_x |= ((uint16_t)data << 8);
        printf("data was %d\n", acc_x);
    }*/
#define TOUCH_CHIP_ID 0x00

    uint8_t time_buf[8] = {};
    uint8_t cmd = 0x55;
    cmd = DS1307_SECONDS;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val1, 1);
    cmd = DS1307_MINUTES;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val2, 1);
    cmd = DS1307_HOURS;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val3, 1);
    cmd = DS1307_DAY;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val4, 1);
    cmd = DS1307_DATE;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val5, 1);
    cmd = DS1307_MONTH;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val6, 1);
    cmd = DS1307_YEAR;
    i2c_transfer7(I2C1, DS1307_ADDRESS, &cmd, 1, &val7, 1);

    printf("val1: %d, val2: %d , val3: %d , val4: %d , val5: %d , val6: %d , val7: %d, val8: %d , val9: %d  \n",
            translate_bcd_for_print(val1),
            translate_bcd_for_print(val2),
            translate_bcd_for_print(val3),
            translate_bcd_for_print(val4),
            translate_bcd_for_print(val5),
            translate_bcd_for_print(val6),
            translate_bcd_for_print(val7),
            translate_bcd_for_print(val8),
            translate_bcd_for_print(val9)
            );
    return;

    // val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_SECONDS);
    uint32_t i2c = I2C1; uint8_t address = DS1307_ADDRESS; uint8_t reg = DS1307_SECONDS;

    uint32_t timeout = 20000;
    while((I2C_SR2(i2c) & I2C_SR2_BUSY)) {
		if (timeout > 0) {
			timeout--;
		} else {
            printf("I2C_SR2_BUSY ret -1  \n");
            break;
			//return -1;
		}
	}

    if(i2c_start(i2c, address, I2C_WRITE)) {
        printf("ret zero  \n");
        //return 0;
    }
    i2c_send_data(i2c, reg);

    timeout = 20000;
    while(!(I2C_SR1(i2c) & (I2C_SR1_BTF))) {
        if(timeout > 0) {
            timeout--;
        } else {
            printf("I2C_SR1_BTF ret -1  \n");
            break;
            //return -1;
        }
    }

    i2c_start(i2c, address, I2C_READ);

    i2c_send_stop(i2c);

    while(!(I2C_SR1(i2c) & I2C_SR1_RxNE));

    int result = (int) i2c_get_data(i2c);

    I2C_SR1(i2c) &= ~I2C_SR1_AF;
    msleep(1);
    i2c_send_stop(i2c);

    val1 = result;

    /*val2 = (int) i2c_get_data(I2C1);
    val3 = (int) i2c_get_data(I2C1);
    val4 = (int) i2c_get_data(I2C1);
    val5 = (int) i2c_get_data(I2C1);
    val6 = (int) i2c_get_data(I2C1);
    val7 = (int) i2c_get_data(I2C1);*/

//    val2 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_SECONDS);
//    val3 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_MINUTES);
//    val4 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_MINUTES);
    //int result = (int) i2c_get_data(I2C1);
    /*result = (int) i2c_get_data(I2C1);
    result = (int) i2c_get_data(I2C1);
    result = (int) i2c_get_data(I2C1);
    result = (int) i2c_get_data(I2C1);
    result = (int) i2c_get_data(I2C1);
    result = (int) i2c_get_data(I2C1);*/


    //while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));

    i2c_send_stop(I2C1);




    printf("val1: %d, val2: %d , val3: %d , val4: %d , val5: %d , val6: %d , val7: %d, val8: %d , val9: %d  \n", val1, val2, val3, val4, val5, val6, val7, val8, val9);



/*    return;
    if(val1 == -1) printf("S: Timeout "); else printf("S: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_MINUTES);
    if(val1 == -1) printf("M: Timeout "); else printf("M: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_HOURS);
    if(val1 == -1) printf("H: Timeout "); else printf("H: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_DAY);
    if(val1 == -1) printf("D: Timeout "); else printf("D: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_DATE);
    if(val1 == -1) printf("DT: Timeout "); else printf("DT: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_MONTH);
    if(val1 == -1) printf("MO: Timeout "); else printf("MO: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_YEAR);
    if(val1 == -1) printf("Y: Timeout "); else printf("Y: %d ", val1);
    val1 = i2c_read(I2C1, DS1307_ADDRESS, DS1307_CONTROL);
    if(val1 == -1) printf("CRTL: Timeout \n"); else printf("CTRL: %d \n", val1);

    //val = i2c_read(I2C1, DS1307_ADDRESS, DS1307_CONTROL);
    //printf("CTRL: %d\n", val);*/
}

uint8_t DS1307_Init(void) {
    //i2c_write(I2C1, DS1307_ADDRESS, DS1307_CONTROL, 1);

    if(i2c_start(I2C1, DS1307_ADDRESS, I2C_WRITE) != 0)
    {
        printf("i2c_start: Timeout\n");
        return -1;
    }

    i2c_send_data(I2C1, DS1307_CONTROL);

    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));
    i2c_send_data(I2C1, 0);

    while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF)));

    i2c_send_stop(I2C1);

    return 0;

    return 0;
    uint32_t reg32 __attribute__((unused));

    /* Send START condition. */
    printf("i2c_send_start \n");
    i2c_send_start(I2C1);

    /* Waiting for START is send and switched to master mode. */
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
             & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    /* Send destination address. */
    printf("Send destination address \n");
    i2c_send_7bit_address(I2C1, DS1307_ADDRESS << 1, I2C_WRITE);

    /* Waiting for address is transferred. */
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

    /* Cleaning ADDR condition sequence. */
    reg32 = I2C_SR2(I2C1);

    /* Sending the data. */
    printf("Sending the data \n");
    i2c_send_data(I2C1, DS1307_SECONDS); /* OvertemperatureShutdown register */
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));
    i2c_send_data(I2C1, 42); /* MSB */
    //while (!(I2C_SR1(i2c) & I2C_SR1_BTF));
    //i2c_send_data(I2C1, (uint8_t)(temp_os & 0xff00)); /* LSB */
    /* After the last byte we have to wait for TxE too. */
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

    /* Send STOP condition. */
    //printf("Send STOP condition \n");
    i2c_send_stop(I2C1);

    return 0;
}

void DS1307_i2c_init(void)
{
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

    //rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    //rcc_periph_clock_enable(RCC_GPIOC);
    //rcc_periph_clock_enable(RCC_GPIOH);
    rcc_periph_clock_enable(RCC_I2C1);

    // was PA8 - I2C3_SCL
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
    // HAL sets #define  GPIO_MODE_AF_OD  0x00000012U   /*!< Alternate Function Open Drain Mode    */ Is needed!
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8);
    // HAL sets #define GPIO_AF4_I2C1 ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8);

    // was PC9 - I2C3_SDA
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO9);

    i2c_peripheral_disable(I2C1); /* disable i2c during setup */
//    i2c_reset(I2C1);

//    i2c_set_fast_mode(I2C1);
//    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_42MHZ);
    i2c_set_speed(I2C1, i2c_speed_sm_100k, 84);
    //i2c_set_ccr(I2C1, 35);
    //i2c_set_trise(I2C1, 43);

    i2c_set_own_7bit_slave_address(I2C1, 0x32);

    i2c_peripheral_enable(I2C1); /* finally enable i2c */
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
    msleep(1);
    i2c_send_stop(i2c);

    return result;
}


