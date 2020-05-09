//
// Created by Jedzia on 08.05.2020.
//

#ifndef BLINKYCPP_541041345A74C57AAB1C1A34E544FC2A_DEBUG_H
#define BLINKYCPP_541041345A74C57AAB1C1A34E544FC2A_DEBUG_H

#ifdef    __cplusplus
extern "C" {
#endif

#define UNUSED(x) (void)(x)

#if 0
#include <stdarg.h>
#include <stdio.h>

static inline
int	dprintf (const char *__restrict, ...)
_ATTRIBUTE ((__format__ (__printf__, 1, 2)));

int dprintf(const char *fmt, ...) {
    va_list arp;
    va_start(arp, fmt);
    int result = printf(fmt, arp);
    va_end(arp);

    return result;
}
#else

static inline
int	dprintf (const char *__restrict, ...);

int dprintf(const char *fmt, ...) {
    UNUSED(fmt);
    return 0;
}

#endif

#ifdef    __cplusplus
}
#endif

#endif //BLINKYCPP_541041345A74C57AAB1C1A34E544FC2A_DEBUG_H
