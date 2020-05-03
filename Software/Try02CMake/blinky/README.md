# How to build

## ARM build (with MinGW CMake) ##

    $ git submodule update --init --recursive
    
    $ mkdir build/
    $ cd build/
    
    $ cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake ..
    
    $ make libopencm3
    
    $ make 
    
    $ make flash
    $ make upload
    
    $ make monitor
    $ make fmonitor


### Example Toolchain File, `arm-none-eabi.cmake` ###

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR arm)
    SET(CMAKE_CROSSCOMPILING 1)
    #set(CMAKE_C_COMPILER_WORKS 1) # In some cases you have to force compiler detection to work.
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # ToDo: a way to specify different link flags for TryCompile
    
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    
    set(OBJCOPY arm-none-eabi-objcopy)
    set(SIZE arm-none-eabi-size)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    
    #Enable semi hosting
    #set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=rdimon.specs")

### Example Project Configuration File, `local.cmake` ###
Here you can easily tweak the settings used during development such as communication
settings or tool locations.

    # Settings-File for local settings. Copy this template to 'local.cmake' and adjust the
    # paths and settings to your needing.
    
    # Python and serial.tools.miniterm is required for serial monitor functionality.
    set(PYTHON "C:/Python/Python37/python")
    
    # Serial monitor configuration. Used by target 'monitor and 'fmonitor'.
    set(PORT COM17)
    set(BAUDRATE 115200)
    
    # Path to OpenOCD. Used by target 'flash'.
    set(OOCD openocd)
    set(OOCD_FILE board/st_nucleo_f4.cfg)
    
    # Path to st-flash utility. Used by target 'upload'.
    set(STFLASH /home/ppkt/usr/bin/st-flash)


## Alternative Editor, Visual Studio / Code ##
Just as a note that it is possible to use VS/VS-Code for editing. But i don't use it.
> why VS? better use CLion

Visual studio setup (for editing, with Windows CMake)

    cd blinky
    mkdir buildVS
    cd buildVS
    "C:\Program Files\CMake\bin\cmake" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. 
    
