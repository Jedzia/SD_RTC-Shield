/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "SDBlockDevice.h"
#include "USBMSD.h"

// socket. The PINS are:
//     MOSI (Master Out Slave In)
//     MISO (Master In Slave Out)
//     SCLK (Serial Clock)
//     CS (Chip Select)
//SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
//SDBlockDevice sd(PA_7, PA_6, PA_5, PB_6);
//USBMSD_SD sd(PA_7, PA_6, PA_5, PB_6);
SDBlockDevice sd(PA_7, PA_6, PA_5, PB_6);
USBMSD usb(&sd);

int main()
{

    while (true) {
        usb.process();
    }

    return 0;
}
