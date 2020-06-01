# ARM helper macros

# Display size information after build.
# Parameter size_target: your elf target without the .elf file extension.
# Example result:
#    text	   data	    bss	    dec	    hex	filename
#   1974	     28	    166	   2168	    878	....../src/avr_blink.elf
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
endmacro(do_objcopy)

# Create flash target for uploading code via avrdude.
# Parameter _target: your elf target without the .elf file extension.
macro(add_flash_target _target)
    # it seems that the ihex detour is not necessary, but use it anyway for compatibility.
    add_custom_target(objcopy_flash # create flash-ihex
            ${OBJCOPY} -O ihex -R .eeprom "${_target}.elf" ${_target}.eep
            #${OBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "${_target}.elf" ${_target}.eep
            DEPENDS ${_target}.elf
            COMMENT "Invoking: objcopy for flash '${_target}.elf'")

    add_custom_target(flash # upload flash-ihex via avrdude
            ${AVRDUDE} -c ${AVRDUDE_PROGRAMMER} -p ${AVRDUDE_PART} -b ${BAUDRATE} -P ${PORT} -U flash:w:"${_target}.eep"
            DEPENDS objcopy_flash
            COMMENT "Invoking: avrdude flash with ${_target}.eep")

    #add_custom_target(flash # upload the program via avrdude
    #        ${AVRDUDE} -c ${AVRDUDE_PROGRAMMER} -p ${AVRDUDE_PART} -b ${BAUDRATE} -P ${PORT} -U flash:w:"${_target}.elf"
    #        DEPENDS ${_target}.elf
    #        COMMENT "Invoking: avrdude flash with ${_target}.elf")
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


