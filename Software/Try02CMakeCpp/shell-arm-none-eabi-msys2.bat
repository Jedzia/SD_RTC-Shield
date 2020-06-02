@title 'ARM Embedded GCC\9-2019-q4-major-win32\bin MSys2 Shell.'
@echo off
call %~dp0%config\config.bat

set CMAKE_GENERATOR=MSYS Makefiles

C:\msys64\msys2_shell_zsh.cmd -use-full-path -here -mingw64 -c "cat config/WelcomeShell.txt ; cd blinkyCpp/build ; zsh"