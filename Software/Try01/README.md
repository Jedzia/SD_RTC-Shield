
make -C my-project clean all

Easy "clone and go" repository for a libopencm3 based project.

# Instructions
 1. git clone https://github.com/libopencm3/libopencm3-template.git your-project
 2. cd your-project
 3. git submodule update --init # (Only needed once)
 4. make -C libopencm3 # (Only needed once)
 5. make -C my-project

# Directories
* my-project contains your application
* my-common-code contains something shared.

# As a template
You should replace this with your _own_ README if you are using this
as a template.

# Notes #

    E:\Projects\Elektronik\ARM\STM32F401\SD_RTC-Shield\Software\Try03PlatformIO\.pio\libdeps\nucleo_f401re\Adafruit_RTCLib_ID3122\DS1307.h
    E:\Projects\Elektronik\ARM\mbed\tmp\test01