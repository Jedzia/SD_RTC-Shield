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
