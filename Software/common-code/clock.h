/*---------------------------------------------------------*/
/*!
 * This file is part of the SD_RTC Datalogger Shield example repository.
 * License details can be found in the file COPYING.
 * Copyright (c) 2020, EvePanix. All rights reserved.
 *
 * \brief      This include file describes the functions
 *             exported by clock.c
 * \file       clock.h
 * \date       2020-01-06
 * \author     Jedzia.
 *
 * modified    2020-05-27, Jedzia
 */
/*---------------------------------------------------------*/

#ifndef __CLOCK_H
#  define __CLOCK_H

#  ifdef __cplusplus
extern "C" {
#  endif

#  include "stdint.h"

/*
 * Definitions for functions being abstracted out
 */
void msleep(uint32_t);

uint32_t mtime(void);

void sys_clock_setup(void);

/* Called when systick fires */
void clock_sys_tick_handler(void);

#  ifdef __cplusplus
}
#  endif
#endif /* generic header protector */
