get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

message(STATUS "Configuring project ${PROJECT_NAME}")

project(${PROJECT_NAME} C CXX ASM)
cmake_minimum_required(VERSION 3.1)
#set(CMAKE_VERBOSE_MAKEFILE On)

# check if we have a local cmake include file and include it if we do
# this is useful for setting our python location or st-flash as an alternative to
# environment variables
set(PYTHON "C:/Python/Python37/python")
set(PORT COM17)
set(BAUDRATE 115200)
set(OOCD openocd)
set(OOCD_FILE board/st_nucleo_f4.cfg)
set(STFLASH /home/ppkt/usr/bin/st-flash)
set(QEMU "C:/Toolchain/ARM/GNU MCU Eclipse/QEMU/bin/qemu-system-gnuarmeclipse.exe")
if (EXISTS ${CMAKE_SOURCE_DIR}/local.cmake)
    message(STATUS "Using local.cmake")
    include(${CMAKE_SOURCE_DIR}/local.cmake)
endif ()

macro(do_size size_target)
    add_custom_command(TARGET ${size_target}.elf POST_BUILD
            COMMAND ${SIZE} --format=berkeley "$<TARGET_FILE:${size_target}.elf>"
            COMMENT "Invoking: Cross ARM GNU Print Size")
endmacro(do_size)

macro(do_objcopy objcopy_target type)
    set(do_objcopy_cmd binary)
    set(do_objcopy_ext bin)

    if ("${type}" STREQUAL "srec")
        set(do_objcopy_cmd srec)
        set(do_objcopy_ext srec)
    endif ()

    if ("${type}" STREQUAL "hex")
        set(do_objcopy_cmd ihex)
        set(do_objcopy_ext hex)
    endif ()

    add_custom_target(${objcopy_target}.${do_objcopy_ext} ALL
            ${OBJCOPY} -O${do_objcopy_cmd} ${objcopy_target}.elf ${objcopy_target}.${do_objcopy_ext}
            DEPENDS ${objcopy_target}.elf
            COMMENT "Invoking: Cross ARM GNU OBJCOPY ${objcopy_target} to ${do_objcopy_cmd}")
endmacro(do_objcopy)


set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/../libopencm3)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

#set(STM32F4_FLAGS "-mcpu=cortex-m3 -mthumb -msoft-float")
set(STM32F4_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard")
#[[
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "\
-std=c11 \
-Os \
-Wall \
-Wextra \
-Wimplicit-function-declaration \
-Wredundant-decls \
-Wmissing-prototypes \
-Wstrict-prototypes \
-Wundef \
-Wshadow \
-fno-common \
-ffunction-sections \
-fdata-sections \
-Wno-packed-bitfield-compat \
${STM32F4_FLAGS}")
]]
# -Werror \

set(CMAKE_COMMON_FLAGS "\
-O0 -ggdb3 \
-mfpu=fpv4-sp-d16 \
-fno-common -ffunction-sections -fdata-sections \
-MD -Wall -Wundef \
 ${STM32F4_FLAGS}")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "\
-std=c99 \
-Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes \
 ${CMAKE_COMMON_FLAGS} ${STM32F4_FLAGS}")
#-DSTM32F4 -DSTM32F401RET6

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMMON_FLAGS} ${STM32F4_FLAGS} -lc -lm -lstdc++ -lsupc++")
SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CMAKE_C_FLAGS} -x assembler-with-cpp")

#[[
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-T ${CMAKE_CURRENT_SOURCE_DIR}/generated.stm32f401ret6.ld \
-MD \
--static \
-g3 \
-ggdb3 \
-nostartfiles \
-mhard-float \
-lc \
-lgcc \
-lnosys \
-Wl,--gc-sections")
]]

#message( "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-T ${CMAKE_CURRENT_SOURCE_DIR}/generated.stm32f401ret6.ld \
-nostartfiles -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
-Wl,--gc-sections \
-Wl,--start-group \
-lc -lgcc \
-lnosys \
-specs=nosys.specs \
-specs=nano.specs \
-Wl,--end-group")
#
#-lnosys \
#-specs=nano.specs \
# -lc -lgcc \
# semihosting:
# -specs=rdimon.specs -lrdimon \

# Source files for main target.
set(SOURCES
        main.cpp
        #main2.cpp
        # common_lib/utils.c
        ../../common-code/api.c
        ../../common-code/api-asm.S
        ../../common-code/uartprint.c
        ../../common-code/clock.c
        ../../common-code/DS1307.c

        # ChaN's FatFs - Generic FAT Filesystem Module http://elm-chan.org/fsw/ff/00index_e.html
        ../../common-code/fatfs/diskio.h
        # these are the library and example implementations of raw disk access functions
        # ../../common-code/fatfs/diskio.c
        # ../../common-code/fatfs/mmc_stm32f1_spi.c
        ../../common-code/fatfs/mmc_stm32f4_spi.c
        ../../common-code/fatfs/ff.c
        ../../common-code/fatfs/ff.h
        ../../common-code/fatfs/ffconf.h
        ../../common-code/fatfs/ffsystem.c
        ../../common-code/fatfs/ffunicode.c
        ../../common-code/fatfs/xprintf.c
        ../../common-code/fatfs/xprintf.h
        ../../common-code/fatfs/mmc_stm32f4_spi.h)
#set_source_files_properties(../common-code/api-asm.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")


add_definitions(-DSTM32F4 -DSTM32F401RET6 -D_LF_CRLF=1)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}../my-common-code
        ${CMAKE_CURRENT_SOURCE_DIR}../../../
        ${LIBOPENCM3_DIR}/include
)

add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
link_directories(${LIBOPENCM3_DIR}/lib)

add_executable(${CMAKE_PROJECT_NAME}.elf ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}.elf opencm3_stm32f4)
do_objcopy(${CMAKE_PROJECT_NAME} bin)
do_objcopy(${CMAKE_PROJECT_NAME} srec)
do_objcopy(${CMAKE_PROJECT_NAME} hex)
do_size(${CMAKE_PROJECT_NAME})

add_custom_target(flash
        ${OOCD} -f ${OOCD_FILE} -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
        DEPENDS ${CMAKE_PROJECT_NAME}.elf
        COMMENT "Invoking: OpenOCD flash with ${CMAKE_PROJECT_NAME}.elf")

add_custom_target(monitor # Start serial monitor.
        ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
        COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")
add_custom_target(fmonitor # Flash target and start serial monitor.
        ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
        DEPENDS flash
        COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")


add_custom_target(upload # Flash target via 'st-flash'.
        stat -c %s ${CMAKE_PROJECT_NAME}.bin &&
        ${STFLASH} --reset write ${CMAKE_PROJECT_NAME}.bin 0x08000000
        DEPENDS ${CMAKE_PROJECT_NAME}.bin)

# >qemu-system-gnuarmeclipse.exe -board help
#
# Supported boards:
# Maple                LeafLab Arduino-style STM32 microcontroller board (r5)
# NUCLEO-F072RB        ST Nucleo Development Board for STM32 F072 devices
# NUCLEO-F103RB        ST Nucleo Development Board for STM32 F1 series
# NUCLEO-F411RE        ST Nucleo Development Board for STM32 F4 series
# NetduinoGo           Netduino GoBus Development Board with STM32F4
# NetduinoPlus2        Netduino Development Board with STM32F4
# OLIMEXINO-STM32      Olimex Maple (Arduino-like) Development Board
# STM32-E407           Olimex Development Board for STM32F407ZGT6
# STM32-H103           Olimex Header Board for STM32F103RBT6
# STM32-P103           Olimex Prototype Board for STM32F103RBT6
# STM32-P107           Olimex Prototype Board for STM32F107VCT6
# STM32F0-Discovery    ST Discovery kit for STM32F051 line
# STM32F4-Discovery    ST Discovery kit for STM32F407/417 lines
# STM32F429I-Discovery ST Discovery kit for STM32F429/439 lines
# generic              Generic Cortex-M board; use -mcu to define the device
#
# >qemu-system-gnuarmeclipse.exe -mcu help
#
# Supported MCUs:
# STM32F051R8
# STM32F072RB
# STM32F103RB
# STM32F107VC
# STM32F405RG
# STM32F407VG
# STM32F407ZG
# STM32F411RE
# STM32F429ZI
add_custom_target(emu # emulation with QEMU-Arm.
        ${QEMU} --verbose --verbose --board NUCLEO-F411RE --mcu STM32F411RE -d unimp,guest_errors --image ${CMAKE_PROJECT_NAME}.elf
        DEPENDS ${CMAKE_PROJECT_NAME}.bin)
