/*---------------------------------------------------------*/
/*!
 * This file is part of the SD_RTC Datalogger Shield example repository.
 * License details can be found in the file COPYING.
 * Copyright (c) 2020, EvePanix. All rights reserved.
 *
 * \brief      This file contains the startup code
 *             of the application.
 * \file       main.cpp
 * \date       2020-01-01
 * \author     Jedzia.
 *
 * modified    2020-01-01, Jedzia
 */
/*---------------------------------------------------------*/
#include "mbed.h"
//
#include "USBMSD_SD.h"
//#include "DS1307.h"
//#include "FATFileSystem.h"
//#include "SDBlockDevice.h"

//#include "USBSDFileSystem.h"


/*RTC*/
#define I2C1_SDA PB_9
#define I2C1_SCL PB_8


// socket. The PINS are:
//     MOSI (Master Out Slave In)
//     MISO (Master In Slave Out)
//     SCLK (Serial Clock)
//     CS (Chip Select)
//SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
//SDBlockDevice sd(PA_7, PA_6, PA_5, PB_6);
//USBMSD_SD sd(PA_7, PA_6, PA_5, PB_6);

// LPC
USBMSD_SD sd(p5, p6, p7, p8);


int main() {
    while(1);
}

// main
