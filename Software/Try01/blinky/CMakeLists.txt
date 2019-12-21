get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

message(STATUS "Configuring project ${PROJECT_NAME}")

project(${PROJECT_NAME} C ASM)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=rdimon.specs")


set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/../libopencm3)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# -Werror \

#set(STM32F4_FLAGS "-mcpu=cortex-m3 -mthumb -msoft-float")
set(STM32F4_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard")
set(EXTRA_C_FLAGS "-std=c11 \
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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-T ${CMAKE_CURRENT_SOURCE_DIR}/generated.stm32f401ret6.ld \
-MD \
--static \
-g3 \
-ggdb3 \
-nostartfiles \
-msoft-float \
-lc \
-lgcc \
-lnosys \
-Wl,--gc-sections")

set(SOURCES
    main.c
    common_lib/utils.c
    )

add_definitions(-DSTM32F4)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBOPENCM3_DIR}/include
)

add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
link_directories(${LIBOPENCM3_DIR}/lib)

add_executable(${CMAKE_PROJECT_NAME}.elf ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME}.elf
    opencm3_stm32f4)

add_custom_target(${CMAKE_PROJECT_NAME}.bin ALL
    ${OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin
    DEPENDS ${CMAKE_PROJECT_NAME}.elf)


add_custom_target(upload
    stat -c %s ${CMAKE_PROJECT_NAME}.bin &&
    /home/ppkt/usr/bin/st-flash --reset write ${CMAKE_PROJECT_NAME}.bin 0x08000000
    DEPENDS ${CMAKE_PROJECT_NAME}.bin)
