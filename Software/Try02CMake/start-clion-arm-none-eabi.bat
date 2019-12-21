@title Launching Clion with 'ARM Embedded GCC\9-2019-q4-major-win32\bin' settings.
@echo off
REM set PATH=C:\Toolchain\MIPS\gcc-4.9.2-mips-unknown-linux-gnu\bin;C:\Toolchain\MIPS\gcc-4.9.2-mips-unknown-linux-gnu\libexec\gcc\mips-unknown-linux-gnu\4.9.2;%PATH%
REM set PATH=C:\Toolchain\ARM\GNU Tools ARM Embedded\8 2018-q4-major\bin;C:\Toolchain\mingw-w64\x86_64-6.2.0-posix-seh-rt_v5-rev1\mingw64\bin;C:\msys64\usr\bin;%PATH%
REM set PATH=C:\Toolchain\ARM\GNU Tools ARM Embedded\8 2018-q4-major\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%
REM set PATH=C:\Toolchain\ARM\gcc-arm-none-eabi-4_8-140708\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%
REM set PATH=D:\Users\Jedzia.pubsiX\AppData\Local\Arduino15\packages\arduino\tools\arm-none-eabi-gcc\4.8.3-2014q1\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%
REM set PATH=C:\Toolchain\ARM\GNU MCU Eclipse\ARM Embedded GCC\8.2.1-1.4-20190214-0604\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%
set PATH=C:\Toolchain\ARM\GNU MCU Eclipse\ARM Embedded GCC\9-2019-q4-major-win32\bin;C:\Toolchain\mingw-w64\x86_64-w64-mingw32-8.3.0-crt-6.0.0-multilib\bin;C:\msys64\usr\bin;C:\Toolchain\OpenOCD\OpenOCD-20190210-0.10.0\bin;%PATH%

REM call C:\Toolchain\ARM\GNU MCU Eclipse\ARM Embedded GCC\9-2019-q4-major-win32\bin\gccvar.bat

REM set TOOL_PATH=/C/Toolchain/ARM/GNU Tools ARM Embedded/8 2018-q4-major/bin

REM Used by gdb to refer to your master C:\Users\<USER>\.gdbinit
set HOME=%USERPROFILE%

REM set CC=arm-none-eabi-gcc
REM set CFLAGS_arm-none-eabi=-EL -mips32 -msoft-float
REM set AR_arm-none-eabi=arm-none-eabi-ar

REM set MAKEFLAGS=-j8

REM call C:\D\ldc2vars64.bat
rem cd E:\Projects\D

start "" "C:\Program Files\JetBrains\CLion 2019.2.3\bin\clion64.exe"

@REM sleep 8 seconds
@PING localhost -n 9 >NUL
