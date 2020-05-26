REM SET CMAKE_DIR=D:\Program Files (x86)\CMake

REM SET ZLIBVER=1.2.8
REM SET SDLVER=2.0.4

REM if "%BOOST_ROOTDIR%"=="" (
REM REM SET BOOST_ROOTDIR=E:\Devel\CPP\Libs\boost\boost-1.55.0
REM SET BOOST_ROOTDIR=E:\Devel\CPP\Libs\boost\boost-git_latest
REM REM SET BOOST_ROOTDIR=E:\Devel\CPP\Libs\boost\boost_1_58_0
REM REM SET BOOST_ROOTDIR=E:\Devel\CPP\Libs\boost\boost_1_59_0
REM REM SET BOOST_ROOTDIR=E:\Devel\CPP\Libs\boost\boost_1_60_0
REM )
REM SET XSD_ROOTDIR=E:\Devel\CPP\Libs\CodeSynthesisXSD-3.3

set PATH=C:\Toolchain\ARM\GNU MCU Eclipse\ARM Embedded GCC\9-2019-q4-major-win32\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%

REM call C:\Toolchain\ARM\GNU MCU Eclipse\ARM Embedded GCC\9-2019-q4-major-win32\bin\gccvar.bat
REM set TOOL_PATH=/C/Toolchain/ARM/GNU Tools ARM Embedded/8 2018-q4-major/bin

REM set CC=arm-none-eabi-gcc
REM set CFLAGS_arm-none-eabi=-EL -mips32 -msoft-float
REM set AR_arm-none-eabi=arm-none-eabi-ar
REM set MAKEFLAGS=-j8
