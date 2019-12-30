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
#include "DS1307.h"
#include "mbed.h"
//#include    "FATFileSystem.h"
//#include    "SDBlockDevice.h"
//#include "FATFileSystem.h"
//FileSystemHandle fs;

//#include <features/storage/filesystem/fat/FATFileSystem.h>
//#include "filesystem/mbed_filesystem.h"

//#include "filesystem/fat/FATFileSystem.h"

//#include "BlockDevice.h"
// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10

// This will take the system's default block device
//BlockDevice *bd = BlockDevice::get_default_instance();

//#include "FATFileSystem.h"
//FATFileSystem fs("fs");

//SDFileSystem sd(PA_7, PA_6, PA_5, PB_6, "sd"); // the pinout on the mbed Cool Components workshop
// board

#include "SDBlockDevice.h"

// Instantiate the SDBlockDevice by specifying the SPI pins connected to the SDCard
// socket. The PINS are:
//     MOSI (Master Out Slave In)
//     MISO (Master In Slave Out)
//     SCLK (Serial Clock)
//     CS (Chip Select)
//SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK,
// MBED_CONF_SD_SPI_CS);
SDBlockDevice sd(PA_7, PA_6, PA_5, PB_6);

#include "FATFileSystem.h"

FATFileSystem fs("fs");

/*RTC*/
#define I2C1_SDA PB_9
#define I2C1_SCL PB_8

inline void wait_msec(unsigned int ms);

/** Entry function
 *  The application starts here.
 *  @return This function never returns.
 */
int main() {
    DigitalOut myLed(LED1);
    Serial usart2(PA_2, PA_3);
    int count = 42;

    usart2.baud(115200);
    printf("Up and running.\n");

    /*init RTC, get time and date for filename*/
    I2C i2c1(I2C1_SDA, I2C1_SCL);
    i2c1.frequency(100000);

    RtcDs1307 rtc(i2c1);

    if(!rtc.isRunning()) {
        usart2.printf("RTC 1 is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    DateTime dt = rtc.now();                     // check clock value
    if(dt.year() > 2090 || dt.year() < 2014) {
        usart2.printf("Setting up time\n");
        DateTime compiled(__DATE__, __TIME__);      // to set RT clock initially
        compiled = DateTime(compiled.unixtime() + 10);
        rtc.adjust(compiled);
        //DateTime dt = rtc.now();                // check again
    }

    //set_time(0);                        // set active clock

    if(!rtc.isRunning()) {
        usart2.printf("RTC 2 is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    if(0 != sd.init()) {
        printf("SD Init failed \n");
        //return -1;
    }
    else {
        printf("sd size: %llu\n", sd.size());
        printf("sd read size: %llu\n", sd.get_read_size());
        printf("sd program size: %llu\n", sd.get_program_size());
        printf("sd erase size: %llu\n", sd.get_erase_size());
    }

    // Set the frequency
    if(0 != sd.frequency(5000000)) {
        printf("Error setting frequency \n");
    }

    int err = fs.mount(&sd);
    if(err) {
        printf("%s\n", (err ? "Fail :(" : "OK"));
    }
    else {
        // Display the root directory
        printf("Opening the root directory... ");
        fflush(stdout);
        DIR* d = opendir("/fs/");
        printf("%s\n", (!d ? "Fail :(" : "OK"));
        if(!d) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }

        printf("root directory:\n");

        while(true) {
            struct dirent* e = readdir(d);
            if(!e) {
                break;
            }

            printf("    %s\n", e->d_name);
        }
        printf("Closing the root directory... ");
        fflush(stdout);
        err = closedir(d);
        printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
        if(err < 0) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }
    }

//    dp = opendir("/sd");
//    if(dp == nullptr) {
//        usart2.printf("Failed to open SD Card.");
//    } else {
//        auto xxx = dp->size();
//        usart2.printf("dp size:%d\n", 5);
//
//        //int err = fs.mount(bd);
//        //printf("%s\n", (err ? "Fail :(" : "OK"));
//
//        /*mkdir("/sd/mydir", 0777);
//
//        FILE *fp = fopen("/sd/mydir/sdtest.txt", "w");
//        if(fp == nullptr) {
//            error("Could not open file for write\n");
//        }
//        fprintf(fp, "Hello fun SD Card World!");
//        fclose(fp);
//
//        printf("Goodbye World!\n");*/
//    }

    while(true) {
        myLed = 1;
        wait_msec(500);
        myLed = 0;
        wait_msec(500);

        usart2.printf("Round %d\n", count);
        count++;

        dt = rtc.now();
        usart2.printf("%u.%u.%02u_%02u.%02u.%02u", dt.month(), dt.day(), dt.year(), dt.hour(), dt.minute(),
                dt.second());
    }
} // main

/** Brief description of $(fclass), wait_msec
 *  Detailed description.
 *  @param ms TODO
 */
inline void wait_msec(const unsigned int ms) {
    wait_us(ms * 1000);
}

// main
