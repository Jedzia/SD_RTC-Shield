//
// Created by Jedzia on 24.12.2019.
//

#include <errno.h>
#include <sys/unistd.h>
#include <libopencm3/stm32/usart.h>

__attribute__ ((used)) ssize_t _write(int file, const char *ptr, ssize_t len);
__attribute__ ((used)) ssize_t _write(int file, const char *data, ssize_t len)
{
    //usart_send_blocking(USART2, '*');
    int i;

    if (file == 1) {
        for (i = 0; i < len; i++) {
            if (data[i] == '\n') {
                usart_send_blocking(USART2, '\r');
            }
            usart_send_blocking(USART2, data[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}



