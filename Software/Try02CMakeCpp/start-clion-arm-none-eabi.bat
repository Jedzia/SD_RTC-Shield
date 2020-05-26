@title Launching Clion with 'ARM Embedded GCC\9-2019-q4-major-win32\bin' settings.
@echo off
call %~dp0%config\config.bat

set HOME=%USERPROFILE%

start "" "C:\Program Files\JetBrains\CLion\bin\clion64.exe"

@REM sleep 8 seconds
@PING localhost -n 9 >NUL
