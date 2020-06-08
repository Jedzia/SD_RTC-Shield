# How to build

## Build for ARM (with MinGW CMake) ##

Under Windows you may have to specify the generator with `-G"MinGW Makefiles"` and use 
**mingw32-make**. This depends on the method you use and your development setup.

- Fetch or update libopencm3: 
    
        $ git submodule update --init --recursive

- Create a build directory 

        $ mkdir build/
        $ cd build/

- Configure the CMake build system 

        $ cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake ..
        or
        $ cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. -G"MinGW Makefiles"
        
- Build the platform libopencm3 library

        $ make libopencm3
        or in the libopencm3 directory a 
        $ make -j
        
- Build your project with

        $ make 
        
- Get info about available targets with

        $ make help
        
- Then upload it via OpenOCD (flash) or st-flash (upload) 

        $ make flash
        $ make upload
        
- View serial output of the target via monitor program or flash the target before
  monitor invocation in one go (fmonitor).

        $ make monitor
        $ make fmonitor

For MSys or Cygwin you may have to specify the Generator: 

    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. -G"MSYS Makefiles"
    cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. -G"Unix Makefiles"


### Example Toolchain File, `arm-none-eabi.cmake` ###

    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR arm)
    SET(CMAKE_CROSSCOMPILING 1)
    #set(CMAKE_C_COMPILER_WORKS 1) # In some cases you have to force compiler detection to work.
    #set(CMAKE_CXX_COMPILER_WORKS 1) # In some cases you have to force compiler detection to work.
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # ToDo: a way to specify different link flags for TryCompile
    
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    
    set(OBJCOPY arm-none-eabi-objcopy)
    set(SIZE arm-none-eabi-size)
    set(NM arm-none-eabi-nm)
    set(OBJDUMP arm-none-eabi-objdump)
    
    #set(CMAKE_ASM_COMPILER arm-none-eabi-as)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
    set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    
    #Enable semi hosting
    #set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=rdimon.specs")


### Example Project Configuration File, `local.cmake` ###
Here you can easily tweak the settings used during development such as communication
settings or tool locations.

    # Settings-File for local settings. Copy this template to 'local.cmake' and adjust the
    # paths and settings to your needing.
    
    # Python and serial.tools.miniterm is required for serial monitor functionality.
    set(PYTHON "C:/Python/Python37/python")
    
    # Serial monitor configuration. Used by target 'monitor and 'fmonitor'.
    set(PORT COM17)
    set(BAUDRATE 115200)
    
    # Path to OpenOCD. Used by target 'flash'.
    set(OOCD openocd)
    set(OOCD_FILE board/st_nucleo_f4.cfg)
    
    # Path to st-flash utility. Used by target 'upload'.
    set(STFLASH /home/ppkt/usr/bin/st-flash)
    
    # Path to QEMU Arm Emulator
    set(QEMU "C:/Toolchain/ARM/GNU MCU Eclipse/QEMU/bin/qemu-system-gnuarmeclipse.exe")


## Alternative Editor, Visual Studio / Code ##
Just as a note that it is possible to use VS/VS-Code for editing. But i don't use it.
> why VS? better use CLion

Visual studio setup (for editing, with Windows CMake)

    cd blinky
    mkdir buildVS
    cd buildVS
    "C:\Program Files\CMake\bin\cmake" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi.cmake .. -G"Visual Studio 16 2019"


# Development notes #

## C++ related

from a STM32CubeIDE project:

    # > make all
    # > arm-none-eabi-g++ "../main.cpp" -std=gnu++14 -g3 -c -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"main.d" -MT"main.o" --specs=nano.specs -mfloat-abi=soft -mthumb -o "main.o"
    # > arm-none-eabi-g++ -o "Mcu01.elf" @"objects.list"   --specs=nosys.specs -Wl,-Map="Mcu01.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    # > Finished building target: Mcu01.elf

C++ project:

    # > arm-none-eabi-g++ -o "MyCpp.elf" @"objects.list"   -mcpu=cortex-m4 -T"E:\Projects\Elektronik\ARM\Algobuilder\Design01\Project\CubeIDE\STM32F401RE-Nucleo\MyCpp\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MyCpp.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    # > Finished building target: MyCpp.elf


## Known Problems
via http://elm-chan.org/fsw/ff/patches.html:

> 
> FatFs R0.14 was released at October 14, 2019. These are the verified problems about FatFs R0.14 and its workaround or patch.
> 
> 1. November 10, 2019
> --------------------------------------------------------------------------------------------------
> Old floppy disks formatted with MS-DOS 2.x and 3.x cannot be mounted.
> 
> This is because the boot sector lacks "FAT" string and 0xAA55 signature. To solve this problem,
> apply http://elm-chan.org/fsw/ff/patch/ff14_p1.diff to the ff.c.
>   
    --- ff.c	Mon Oct 14 12:13:52 2019
    +++ ff14_p1.c	Sun Nov 10 23:26:19 2019
    @@ -3290,23 +3290,37 @@
     
     /* Check what the sector is */
     
    -static UINT check_fs (	/* 0:FAT VBR, 1:exFAT VBR, 2:Valid BS but not FAT, 3:Invalid BS, 4:Disk error */
    +static UINT check_fs (	/* 0:FAT VBR, 1:exFAT VBR, 2:Not FAT and valid BS, 3:Not FAT and invalid BS, 4:Disk error */
        FATFS* fs,			/* Filesystem object */
        LBA_t sect			/* Sector to load and check if it is an FAT-VBR or not */
     )
     {
    -	fs->wflag = 0; fs->winsect = (LBA_t)0 - 1;		/* Invaidate window */
    -	if (move_window(fs, sect) != FR_OK) return 4;	/* Load the boot sector */
    -
    -	if (ld_word(fs->win + BS_55AA) != 0xAA55) return 3;	/* Check boot signature (always here regardless of the sector size) */
    +	WORD w, sign;
    +	BYTE b;
     
    -	if (FF_FS_EXFAT && !mem_cmp(fs->win + BS_JmpBoot, "\xEB\x76\x90" "EXFAT   ", 11)) return 1;	/* Check if exFAT VBR */
     
    -	if (fs->win[BS_JmpBoot] == 0xE9 || fs->win[BS_JmpBoot] == 0xEB || fs->win[BS_JmpBoot] == 0xE8) {	/* Valid JumpBoot code? */
    -		if (!mem_cmp(fs->win + BS_FilSysType, "FAT", 3)) return 0;		/* Is it an FAT VBR? */
    -		if (!mem_cmp(fs->win + BS_FilSysType32, "FAT32", 5)) return 0;	/* Is it an FAT32 VBR? */
    +	fs->wflag = 0; fs->winsect = (LBA_t)0 - 1;		/* Invaidate window */
    +	if (move_window(fs, sect) != FR_OK) return 4;	/* Load the boot sector */
    +	sign = ld_word(fs->win + BS_55AA);
    +#if FF_FS_EXFAT
    +	if (sign == 0xAA55 && !mem_cmp(fs->win + BS_JmpBoot, "\xEB\x76\x90" "EXFAT   ", 11)) return 1;	/* It is an exFAT VBR */
    +#endif
    +	b = fs->win[BS_JmpBoot];
    +	if (b == 0xEB || b == 0xE9 || b == 0xE8) {	/* Valid JumpBoot code? (short jump, near jump or near call) */
    +		if (sign == 0xAA55 && !mem_cmp(fs->win + BS_FilSysType32, "FAT32   ", 8)) return 0;	/* It is an FAT32 VBR */
    +		/* FAT volumes formatted with early MS-DOS lack boot signature and FAT string, so that we need to identify the FAT VBR without them. */
    +		w = ld_word(fs->win + BPB_BytsPerSec);
    +		if ((w & (w - 1)) == 0 && w >= FF_MIN_SS && w <= FF_MAX_SS) {	/* Properness of sector size */
    +			b = fs->win[BPB_SecPerClus];
    +			if (b != 0 && (b & (b - 1)) == 0						/* Properness of cluster size */
    +			&& (fs->win[BPB_NumFATs] == 1 || fs->win[BPB_NumFATs] == 2)	/* Properness of number of FATs */
    +			&& ld_word(fs->win + BPB_RootEntCnt) != 0				/* Properness of root entry count */
    +			&& ld_word(fs->win + BPB_FATSz16) != 0) {				/* Properness of FAT size */
    +				return 0;	/* Sector can be presumed an FAT VBR */
    +			}
    +		}
        }
    -	return 2;	/* Valid BS but not FAT */
    +	return sign == 0xAA55 ? 2 : 3;	/* Not an FAT VBR (valid or invalid BS) */
     }
    
> This problem was reported at a meeting.
> 
> 2. March 29, 2020
> --------------------------------------------------------------------------------------------------
> A compiler warning apperes when enable string I/O functions with UTF-8.
> 
> To fix this problem, apply http://elm-chan.org/fsw/ff/patch/ff14_p2.diff to the ff.c with patch 1 applied.
>
    --- ff14_p1.c	Sun Nov 10 23:26:19 2019
    +++ ff14_p2.c	Sun Mar 29 13:56:25 2020
    @@ -6534,7 +6534,7 @@
        WCHAR hs, wc;
     #if FF_LFN_UNICODE == 2
        DWORD dc;
    -	TCHAR *tp;
    +	const TCHAR *tp;
     #endif
     #endif
     
    @@ -6576,7 +6576,7 @@
                return;
            }
        }
    -	tp = (TCHAR*)pb->bs;
    +	tp = (const TCHAR*)pb->bs;
        dc = tchar2uni(&tp);	/* UTF-8 ==> UTF-16 */
        if (dc == 0xFFFFFFFF) return;	/* Wrong code? */
        wc = (WCHAR)dc;
        
> This problem was reported via user forum.
>
>      #3457: BUG: passing "TCHAR **" to "tchar2uni" function                       3/28/2020, 19:50 UTC
>      Name: StefDrums @Internet
>       
>      
>      Dear,
>      I foud a little BUG when using FF_LFN_UNICODE = 2 config.
>      at line 6566 of ff.c shuld pass to "tchar2uni" function a "const TCHAR** str"
>      
>      line 6523: TCHAR *tp;
>      line 6566: dc = tchar2uni(&tp); /* UTF-8 ==> UTF-16 */
>      
>      i suggest to change line 6523 in:
>      line 6523: const TCHAR *tp;
>      
>      this is the warning given by keil ARM IDE:
>      FatFs\ff.c(6566): warning: argument of type "TCHAR **" is incompatible with parameter of type "const TCHAR **"
>      
>      Thanks for your great work!
> 

> 3. April 5, 2020
> --------------------------------------------------------------------------------------------------
> A buffer overflow can occure in the f_mkfs funciton when FF_MIN_SS < FF_MAX_SS.
> The function description had said that the size of working buffer may be the sector size corresponds to
> the volume but the function always uses the buffer in FF_MAX_SS bytes.
> 
> Workaround: Give work area in size of FF_MAX_SS bytes at least.
> 
> This problem was reported via user forum.
>
>       #3458: bug in create_partition (ff.c) during f_mkfs call                4/1/2020, 14:22 UTC
>       Name: Pieter @Internet
>       Child: #3459 
>       
>       Hi
>       
>       I call f_mkfs with my current sector size as the work area buffer.
>       This seems to be ok based on the api documentation and  implementation of f_mkfs but in `create_partion` it calls `mem_set(buf, 0, FF_MAX_SS)`.
>       This causes an overflow because my current sector size (2048) < FF_MAX_SS (4096) so I would propose the following patch
>       
>       --- a/src/fat/src/ff.c
>       +++ b/src/fat/src/ff.c
>       @@ -5791,7 +5791,7 @@ static FRESULT create_partition (
>                       for (n_hd = 8; n_hd != 0 && sz_drv32 / n_hd / n_sc > 1024; n_hd *= 2) ;
>                       if (n_hd == 0) n_hd = 255;      /* Number of heads needs to be <256 */
>       
>       -               mem_set(buf, 0, FF_MAX_SS);     /* Clear MBR */
>       +               mem_set(buf, 0, ss);    /* Clear MBR */^M
>                       pte = buf + MBR_Table;  /* Partition table in the MBR */
>                       for (i = 0, s_lba32 = n_sc; i < 4 && s_lba32 != 0 && s_lba32 < sz_drv32; i++, s_lba32 += n_lba32) {
>                               n_lba32 = (DWORD)plst[i];       /* Get partition size */
>       



## FatFS stuff as reference and to study ##
* http://stm32f4-discovery.net/2014/07/library-21-read-sd-card-fatfs-stm32f4xx-devices/#
    - Apr 2, 2020 https://github.com/MaJerle/stm32f429/blob/master/00-STM32F429_LIBRARIES/tm_stm32f4_fatfs.h
* Jul 22, 2019 https://github.com/ksarkies/Battery-Management-System/blob/master/chan-fat-stm32-loc3/sd_spi_loc3_stm32.c
* Oct 15, 2018 https://github.com/DeviationTX/fatfs-stm32/blob/nemuisan/source/mmc_stm32f4.c
* Sep 30, 2015  https://github.com/tmolteno/openfat/compare/master...mox-mox:master
  > OpenFAT created long filenames are invisible to Windows.
  > warning: 'offset' may be used uninitialized in this function  

  https://github.com/tmolteno/openfat/blob/master/stm32/example.c                                                                       
* Aug 16, 2013 https://github.com/mfauzi/STM32F4/tree/master/STM32F4%20Standard%20Peripheral%20Library/Project/STM32F4xx_StdPeriph_Examples/SDIO/SDIO_uSDCard

## Karols Stuff ##
* https://github.com/ppkt/common_lib