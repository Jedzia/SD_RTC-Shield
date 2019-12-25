get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

message(STATUS "Configuring project ${PROJECT_NAME}")

project(${PROJECT_NAME} C ASM)
cmake_minimum_required(VERSION 3.1)

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

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "\
-O0 -std=c99 -ggdb3 \
-mfpu=fpv4-sp-d16 \
-fno-common -ffunction-sections -fdata-sections \
-Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes \
-MD -Wall -Wundef \
 ${STM32F4_FLAGS}")
#-DSTM32F4 -DSTM32F401RET6


SET(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} "${CMAKE_C_FLAGS} -x assembler-with-cpp")

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
        main.c
#        common_lib/utils.c
        ../common-code/api.c
        ../common-code/api-asm.S
        ../common-code/uartprint.c
        ../common-code/clock.c
        ../common-code/DS1307.c)
#set_source_files_properties(../common-code/api-asm.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")


add_definitions(-DSTM32F4 -DSTM32F401RET6)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}../my-common-code
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

set(OOCD openocd)
set(OOCD_FILE board/st_nucleo_f4.cfg)
add_custom_target(flash
        ${OOCD} -f ${OOCD_FILE} -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
        DEPENDS ${CMAKE_PROJECT_NAME}.elf
        COMMENT "Invoking: OpenOCD flash with ${CMAKE_PROJECT_NAME}.elf")

set(PYTHON "C:/Python/Python37/python")
set(PORT COM17)
set(BAUDRATE 115200)
add_custom_target(monitor
        ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
        COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")
add_custom_target(fmonitor
        ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
        DEPENDS flash
        COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")


add_custom_target(upload
        stat -c %s ${CMAKE_PROJECT_NAME}.bin &&
        /home/ppkt/usr/bin/st-flash --reset write ${CMAKE_PROJECT_NAME}.bin 0x08000000
        DEPENDS ${CMAKE_PROJECT_NAME}.bin)
