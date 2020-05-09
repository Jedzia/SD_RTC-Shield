#include <sched.h>
//#include "api.h"
//#include "api-asm.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <common-code/DS1307.h>
#include <common-code/clock.h>

//#include <common-code/fatfs/xprintf.h>
#include <common-code/fatfs/ff.h>
#include <common-code/fatfs/diskio.h>
#include <common-code/debug.h>

#define USE_RTC 1
const int DELAY_TIME = 1000;

FATFS FatFs;                /* File system object for each logical drive */
FIL File[2];                /* File objects */
DIR Dir;                    /* Directory object */

static void clock_setup(void) {
    /* Enable GPIOA clock. */
    /* Manually: */
    /*RCC_AHB1ENR |= RCC_AHB1ENR_IOPAEN; */
    /* Using API functions: */
    /* Enable GPIOA clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOA);
    /* Enable GPIOB clock for LED. */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Enable clocks for USART2. */
    rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void) {
    /* Setup USART2 parameters. */
    // USART_TX is PA2, USART_RX is PA3. not connected. bridges are open. on Morpho:
    // USART_TX is CN10-35, USART_RX is CN10-37
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART2);
}

static void gpio_setup(void) {
    /* Set GPIO5 (in GPIO port A) to 'output push-pull'. */
    /* Manually: */
    /*GPIOA_CRH = (GPIO_CNF_OUTPUT_PUSHPULL << (((8 - 8) * 4) + 2)); */
    /*GPIOA_CRH |= (GPIO_MODE_OUTPUT_2_MHZ << ((8 - 8) * 4)); */
    /* Using API functions: */
    //gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

    // Both LED's
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

    // PA3 - Arduino D2
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);

    /* Setup GPIO pins for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART2 TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

/*void wait(void);

void wait() {
    for(int i = 0; i < 20 * 120000; i++) {    // Wait a bit.
        __asm__("nop");
    }
}*/

//// Stubs that need implementation for FatFS to work.
///*-----------------------------------------------------------------------*/
///* Initialize disk drive                                                 */
///*-----------------------------------------------------------------------*/
//DSTATUS disk_initialize (
//        __unused BYTE drv		/* Physical drive number (0) */
//)
//{
//    return STA_NOINIT;
//}
//
//
///*-----------------------------------------------------------------------*/
///* Get disk status                                                       */
///*-----------------------------------------------------------------------*/
//DSTATUS disk_status (
//        __unused BYTE drv		/* Physical drive number (0) */
//)
//{
//    //if (drv) return STA_NOINIT;		/* Supports only drive 0 */
//
//    //return Stat;	/* Return disk status */
//    return STA_NOINIT;
//}
//
///*-----------------------------------------------------------------------*/
///* Read sector(s)                                                        */
///*-----------------------------------------------------------------------*/
//DRESULT disk_read (
//        __unused BYTE drv,		/* Physical drive number (0) */
//        __unused BYTE *buff,		/* Pointer to the data buffer to store read data */
//        __unused LBA_t sector,	/* Start sector number (LBA) */
//        __unused UINT count		/* Number of sectors to read (1..128) */
//)
//{
//    return RES_NOTRDY;
//}
//
///*-----------------------------------------------------------------------*/
///* Write sector(s)                                                       */
///*-----------------------------------------------------------------------*/
//DRESULT disk_write (
//        __unused BYTE drv,			/* Physical drive number (0) */
//        __unused const BYTE *buff,	/* Ponter to the data to write */
//        __unused LBA_t sector,		/* Start sector number (LBA) */
//        __unused UINT count			/* Number of sectors to write (1..128) */
//)
//{
//    return RES_NOTRDY;
//}

extern void disk_timerproc(void);

/*---------------------------------------------*/
/* 1kHz timer process                          */
/*---------------------------------------------*/

volatile UINT Timer;

__used void sys_tick_handler(void);

__used void sys_tick_handler(void) {
    clock_sys_tick_handler();
    static uint16_t led;


    Timer++;    /* Increment performance counter */

    if(++led >= 500) {
        led = 0;
        //GPIOC_ODR ^= _BV(9)|_BV(8);		/* Flip Green/Blue LED state */
        gpio_toggle(GPIOB, GPIO3);    /* LED on/off */

    }


    //if(((SPI1_SR & 0x83) != 0x03))
    /*if(gpio_get(GPIOB, GPIO6))
        gpio_set(GPIOB, GPIO3);
    else
        gpio_clear(GPIOB, GPIO3);*/

    disk_timerproc();    /* Disk timer process */
    //gpio_toggle(GPIOA, GPIO10); /* Arduino D2 on/off */

}

// bad hack to print status, remove this after R&D
extern volatile DSTATUS Stat;

static const char *ft[] = {"", "FAT12", "FAT16", "FAT32", "exFAT"};
BYTE Buff[4096] __attribute__ ((aligned (4)));    /* Working buffer */
DWORD AccSize;                /* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;


static inline void xprint_impl(unsigned char c) {
    //printf("%c", c);
    putchar(c);
}

static
void put_rc(FRESULT rc) {
    const char *str =
            "OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
            "INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
            "INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
            "LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0" "INVALID_PARAMETER\0";
    FRESULT i;

    for(i = 0; i != rc && *str; i++) {
        while(*str++);
    }
    printf("rc=%u FR_%s\n", (UINT) rc, str);
}

static
FRESULT scan_files(
        char *path        /* Pointer to the path name working buffer */
) {
    DIR dirs;
    FRESULT res = 0;
    BYTE i = 0;

    if((res = f_opendir(&dirs, path)) == FR_OK) {
        i = strlen(path);
        while(((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
            if(Finfo.fattrib & AM_DIR) {
                AccDirs++;
                *(path + i) = '/';
                //strcpy(path + i + 1, Finfo.fname);
                strcpy(path + i + 1, Finfo.fname);
                res = scan_files(path);
                printf("[scan_files] path='%s'\n", path);
                *(path + i) = '\0';
                if(res != FR_OK) break;
            } else {
                /*	xprintf("%s/%s\n", path, fn); */
                AccFiles++;
                AccSize += Finfo.fsize;
            }
        }
    }

    return res;
}


int ShowVolumeStatus(FATFS *fs, const TCHAR *path);

int ShowVolumeStatus(FATFS *fs, const TCHAR *path) {
    long p1 = 0, p2 = 0, p3 = 0;
    //char *ptr = NULL, *ptr2 = NULL;
    BYTE res = 0, b = 0, drv = 0;

//    while (*ptr == ' ') ptr++;
#if FF_FS_MINIMIZE == 0
    res = f_getfree(path, (DWORD *) &p1, &fs);
    if(res) {
        put_rc(res);
        return -1;
    }
#endif

    printf("FAT type = %s\n", ft[fs->fs_type]);
    printf("Bytes/Cluster = %lu\n", (DWORD) fs->csize * 512);
    printf("Number of FATs = %u\n", fs->n_fats);
    if(fs->fs_type < FS_FAT32) printf("Root DIR entries = %u\n", fs->n_rootdir);
    printf("Sectors/FAT = %lu\n", fs->fsize);
    printf("Number of clusters = %lu\n", (DWORD) fs->n_fatent - 2);
    printf("Volume start (lba) = %lu\n", fs->volbase);
    printf("FAT start (lba) = %lu\n", fs->fatbase);
    printf("DIR start (lba,clustor) = %lu\n", fs->dirbase);
    printf("Data start (lba) = %lu\n\n", fs->database);
#if FF_USE_LABEL
    res = f_getlabel(path, (char *) Buff, (DWORD *) &p2);
    if(res) {
        put_rc(res);
        return -2;
    }
    printf(Buff[0] ? "Volume name is %s\n" : "No volume label\n", (char *) Buff);
    printf("Volume S/N is %04lX-%04lX\n", (DWORD) p2 >> 16, (DWORD) p2 & 0xFFFF);
#endif
    
#if FF_FS_MINIMIZE < 2
    AccSize = AccFiles = AccDirs = 0;
    printf("...");
    strcpy((char *) Buff, path);
    res = scan_files((char *) Buff);
    if(res) {
        put_rc(res);
        return -3;
    }
    printf("\r%u files, %lu bytes.\n%u folders.\n"
            "%lu KiB total disk space.\n%lu KiB available.\n",
            AccFiles, AccSize, AccDirs,
            (fs->n_fatent - 2) * (fs->csize / 2), (DWORD) p1 * (fs->csize / 2)
    );
#endif

    return 0;
}

/*void ShowDiskStatus(FATFS *fs);

void ShowDiskStatus(FATFS *fs) {
    long p1 = 0, p2 = 0, p3 = 0;

    if(disk_ioctl((BYTE) p1, GET_SECTOR_COUNT, &p2) == RES_OK) { xprintf("Drive size: %lu sectors\n", p2); }
}*/

int main(void) {

    // int i, j = 0, c = 0;

    sys_clock_setup();
    clock_setup();
    gpio_setup();
    usart_setup();
    //xfunc_out = xprint_impl;

    //wait();
    msleep(2000);
    printf("\n\nHello, we're running\n");
    //msleep(100);
#if USE_RTC
    i2c_setup();
    //DS1307_i2c_init();
    msleep(100);

    printf("Initializing RTC ...\n");
    if(!DS1307_IsRunning()) {
        printf("RTC is NOT running!\n");
        printf("Running DS1307_Init\n");
        msleep(500);
        DS1307_Init();
    }
#endif

    printf("Initializing Fat File System ...\n");
    // mount immediately
    DebugFS();
    f_mount(&FatFs, "0:", 1);
    //xprintf("FAT type = %s\n", ft[FatFs.fs_type]);
    ShowVolumeStatus(&FatFs, "0:");

    /* Blink the LED (PC8) on the board. */
    while(1) {
        /* Manually: */
        /* GPIOA_BSRR = GPIO5; */        /* LED off */
        /* for (i = 0; i < 1000000; i++) */    /* Wait a bit. */
        /*	__asm__("nop"); */
        /* GPIOA_BRR = GPIO5; */        /* LED on */
        /* for (i = 0; i < 1000000; i++) */    /* Wait a bit. */
        /*	__asm__("nop"); */

        /* Using API functions gpio_set()/gpio_clear(): */
        /* gpio_set(GPIOA, GPIO5); */    /* LED off */
        /* for (i = 0; i < 1000000; i++) */    /* Wait a bit. */
        /*	__asm__("nop"); */
        /* gpio_clear(GPIOA, GPIO5); */    /* LED on */
        /* for (i = 0; i < 1000000; i++) */    /* Wait a bit. */
        /*	__asm__("nop"); */

        /* Using API function gpio_toggle(): */
//        gpio_toggle(GPIOA, GPIO5);    /* LED on/off */

        //    printf("Blink %d\n", c);
//        usart_send_blocking(USART2, 'X');
//        usart_send_blocking(USART2, (uint16_t) (c + '0')); /* USART2: Send byte. */
//        c = (c == 9) ? 0 : c + 1;    /* Increment c. */
//        if((j++ % 80) == 0) {        /* Newline after line full. */
//            usart_send_blocking(USART2, '\r');
//            usart_send_blocking(USART2, '\n');
//        }


        /*uint32_t tickstart = mtime();
        for(int k = 0; k < 1000000; ++k) {
            __asm__("nop");
        }
        uint32_t currticks = mtime();
        printf("mtime (%lu - %lu) = %lu\n",  currticks, tickstart, currticks - tickstart);*/

        //msleep(3 * DELAY_TIME);

//        gpio_toggle(GPIOB, GPIO3);    /* LED on/off */
        //msleep(5 * DELAY_TIME);

#if USE_RTC
        DS1307_DoSomething();
#endif

        //DebugFS();
        gpio_toggle(GPIOB, GPIO5);    /* LED on/off */
        msleep(DELAY_TIME);
        printf("SD stat %d\n", Stat);

    }

    //return 0;

    /* add your own code */
    /*uint32_t rev = 0xaabbccdd;
    rev = rev_bytes(rev);
    return my_func(rev);*/
}
