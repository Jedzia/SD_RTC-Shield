//
// Created by Jedzia on 08.05.2020.
//

#ifndef BLINKYCPP_041034BCC2474C57AA1C1634E54BC22D_BLA_H
#define BLINKYCPP_041034BCC2474C57AA1C1634E54BC22D_BLA_H

#ifdef    __cplusplus
extern "C" {
#endif

#define console_puts(x) puts(x);

/*
* put_status(char *)
*
* This is a helper function I wrote to watch the status register
* it decodes the bits and prints them on the console. Sometimes
* the SPI port comes up with the MODF flag set, you have to re-read
* the status port and re-write the control register to clear that.
*/
void put_status(char *m);


void SPIxENABLE(void);

static inline
void FCLK_SLOW(void);

static inline
void FCLK_FAST(void);


void disk_timerproc(void);

#ifdef    __cplusplus
}
#endif

#endif //BLINKYCPP_041034BCC2474C57AA1C1634E54BC22D_BLA_H
