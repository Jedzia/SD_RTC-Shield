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

include(../CMake/Arm.cmake)

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

# Note ${CMAKE_PROJECT_NAME} is the top-level one. Use ${PROJECT_NAME} like above for the local name.
add_executable(${CMAKE_PROJECT_NAME}.elf ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}.elf opencm3_stm32f4)
do_objcopy(${CMAKE_PROJECT_NAME} bin)
do_objcopy(${CMAKE_PROJECT_NAME} srec)
do_objcopy(${CMAKE_PROJECT_NAME} hex)
do_size(${CMAKE_PROJECT_NAME})

add_flash_target(${CMAKE_PROJECT_NAME})
add_monitor_targets()
add_inspection_targets(${CMAKE_PROJECT_NAME})
add_emulation_target(${CMAKE_PROJECT_NAME} NUCLEO-F411RE STM32F411RE)
