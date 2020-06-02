# ARM helper macros

# Display size information after build.
# Parameter size_target: your elf target without the .elf file extension.
# Example result:
#    text	   data	    bss	    dec	    hex	filename
#   1974	     28	    166	   2168	    878	....../src/blink.elf
macro(do_size size_target)
    add_custom_command(TARGET ${size_target}.elf POST_BUILD
            COMMAND ${SIZE} --format=berkeley "$<TARGET_FILE:${size_target}.elf>"
            COMMENT "Invoking: Cross ARM GNU Print Size")
endmacro(do_size)

# Supply targets to create binary, srec and ihex files via objcopy.
# Parameter objcopy_target: your elf target without the .elf file extension.
# Parameter type: bin, srec or ihex
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
    #add_custom_command(TARGET ${objcopy_target}.elf
    #        COMMAND ${OBJCOPY} -O${do_objcopy_cmd} ${objcopy_target}.elf ${objcopy_target}.${do_objcopy_ext}
    #        DEPENDS ${objcopy_target}.elf
    #        COMMENT "Invoking: Cross ARM GNU OBJCOPY ${objcopy_target} to ${do_objcopy_cmd}")
endmacro(do_objcopy)


# Create targets for uploading code via OpenOCD or st-flash.
# Parameter _target: your elf target without the .elf file extension.
macro(add_flash_target _target)
    add_custom_target(flash
            ${OOCD} -f ${OOCD_FILE} -c "program ${_target}.elf verify reset exit"
            DEPENDS ${_target}.elf
            COMMENT "Invoking: OpenOCD flash with ${_target}.elf")

    add_custom_target(upload # Flash target via 'st-flash'.
            stat -c %s ${_target}.bin &&
            ${STFLASH} --reset write ${_target}.bin 0x08000000
            DEPENDS ${_target}.bin)
endmacro(add_flash_target _target)

# Start a simple Serial-Console-Monitor with 'monitor' or 'fmonitor' to flash the chip before.
macro(add_monitor_targets)
    add_custom_target(monitor # Start serial monitor.
            ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
            COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")
    add_custom_target(fmonitor # Flash target and start serial monitor.
            ${PYTHON} -m serial.tools.miniterm --raw --eol CRLF --encoding ascii ${PORT} ${BAUDRATE}
            DEPENDS flash
            COMMENT "Invoking: miniterm on port ${PORT}. Exit with CTRL+'+'")
endmacro(add_monitor_targets)

# Create targets to provide map and listing generation via objdump.
# Parameter _target: your elf target without the .elf file extension.
macro(add_inspection_targets _target)
    add_custom_target(listing # Assembler listing
            ${OBJDUMP} -S "${_target}.elf" > ${_target}.lst
            DEPENDS ${_target}.elf
            COMMENT "Creating assembly listing of '${_target}.elf'")

    add_custom_target(map # Map file with size information
            COMMAND ${OBJDUMP} -t "${_target}.elf" > ${_target}.map
            COMMAND ${SIZE} --format=berkeley "$<TARGET_FILE:${_target}.elf>" >> ${_target}.map
            DEPENDS ${_target}.elf
            COMMENT "Creating assembly listing of '${_target}.elf'")

    add_custom_target(nm_size # Map file with size information
            COMMAND ${NM} --print-size --size-sort --radix=d "${_target}.elf" > ${_target}.nm.txt
            COMMAND ${SIZE} --format=berkeley "$<TARGET_FILE:${_target}.elf>" >> ${_target}.nm.txt
            DEPENDS ${_target}.elf
            COMMENT "Creating size listing of '${_target}.elf'")
endmacro(add_inspection_targets _target)

# QEMU emulator integration
# Parameter _target: your elf target without the .elf file extension.
# Parameter _board: Board identifier for visual emulation. See list below.
# Parameter _mcu: Micro-Controller-Type to emulate. See list below.
macro(add_emulation_target _target _board _mcu)
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
            ${QEMU} --verbose --verbose --board ${_board} --mcu ${_mcu} -d unimp,guest_errors --image ${_target}.elf
            DEPENDS ${_target}.elf)
endmacro(add_emulation_target _target _board _mcu)
