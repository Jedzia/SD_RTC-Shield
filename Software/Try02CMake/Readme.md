ARM build (with MinGW CMake)

    cd blinky
    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake ..

    make libopencm3

    make
    make clean VERBOSE=1 all
    

why VS? better use CLion

Visual studio setup (for editing, with Windows CMake)

    cd blinky
    mkdir buildVS2015
    cd buildVS2015
    "D:\Program Files (x86)\CMake\bin\cmake" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. 
    