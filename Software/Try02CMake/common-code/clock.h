/*
 * This include file describes the functions exported by clock.c
 */
#ifndef __CLOCK_H
#define __CLOCK_H

/*
 * Definitions for functions being abstracted out
 */
void msleep(uint32_t);
uint32_t mtime(void);
void sys_clock_setup(void);
/* Called when systick fires */
void clock_sys_tick_handler(void);

#endif /* generic header protector */

