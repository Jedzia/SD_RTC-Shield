# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(
            STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE
            RelWithDebInfo
            CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui, ccmake
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()

#set(CMAKE_CXX_COMPILER_LAUNCHER "C:\\msys64\\usr\\bin\\ccache.exe")
#set(CMAKE_CXX_COMPILER_LAUNCHER "E:\\Projects\\C++\\oglTemplate\\CMake\\ccache-launcher.bat")
option(ENABLE_CCACHE "Use the Compiler Cache Tool ccache." ON)
if (ENABLE_CCACHE)
    find_program(CCACHE ccache)
    if (CCACHE)
        if ($ENV{CLION_IDE})

            #set(CMAKE_SYSTEM_NAME MSYS2)
            #        set(CMAKE_C_COMPILER_WORKS 1)
            #        set(CMAKE_CXX_COMPILER_WORKS 1)
            #        set(CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\clang++.exe")
            #   set(CMAKE_C_COMPILER "C:\\msys64\\mingw64\\bin\\clang-cl.exe" )
            #   set(CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\clang-cl.exe" )
            #         set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/x86_64-w64-mingw32-g.exe" )
            #         set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/x86_64-w64-mingw32-g++.exe" )
            #      set(CMAKE_C_COMPILER "x86_64-w64-mingw32-g.exe" )
            #      set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++.exe" )


            message(STATUS "using ccache with CLION: ${CCACHE}")
            # ccache and CLion combined do not like to compile C code, allow only C++.
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
            #message(FATAL_ERROR "CMAKE_CXX_COMPILER_LAUNCHER ${CMAKE_CXX_COMPILER_LAUNCHER}")
            #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall -pedantic ${CCACHE_INCLUDES}")
            #set(CMAKE_CXX_COMPILER "ccache")

            #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
            #set(CMAKE_CXX_COMPILER ${CCACHE})


            #message(WARNING "CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}")
            #set(CMAKE_SYSTEM LINUX)
            #message(WARNING "CMAKE_SYSTEM ${CMAKE_SYSTEM}")
            #message(FATAL_ERROR "CLION_IDE $ENV{CLION_IDE}")
        else ()
            message("using ccache")
            set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
            #set(CMAKE_CXX_COMPILER_LAUNCHER echo)
            #set(CMAKE_CXX_COMPILER_LAUNCHER "C:\\msys64\\usr\\bin\\ccache.exe")
            #message(FATAL_ERROR "CMAKE_CXX_COMPILER_LAUNCHER ${CMAKE_CXX_COMPILER_LAUNCHER}")
        endif ()
    else ()
        message("ccache not found cannot use")
    endif ()
endif ()

# Generate compile_commands.json to make it easier to work with clang based
# tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_IPO
        "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)"
        OFF)

if (ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if (result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else ()
        message(SEND_ERROR "IPO is not supported: ${output}")
    endif ()
endif ()


