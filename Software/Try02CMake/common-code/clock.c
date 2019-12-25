/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Chuck McManis <cmcmanis@mcmanis.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Now this is just the clock setup code from systick-blink as it is the
 * transferrable part.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
//#include <libopencm3/cm3/assert.h>
//#include <libopencm3/stm32/rcc.h>
//#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/flash.h>

/* Common function descriptions */
#include "clock.h"

/* milliseconds since boot */
static volatile uint32_t system_millis;

/* Called when systick fires */
__attribute__ ((used))  void sys_tick_handler(void)
{
	system_millis++;
}

/* simple sleep for delay milliseconds */
void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

/* Getter function for the current time */
uint32_t mtime(void)
{
	return system_millis;
}


/*
 * clock_setup(void)
 *
 * This function sets up both the base board clock rate
 * and a 1khz "system tick" count. The SYSTICK counter is
 * a standard feature of the Cortex-M series.
 */
void sys_clock_setup(void)
{
	/* Base board frequency, set to 168Mhz */
    //rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    //rcc_clock_setup_pll(&rcc_ahb_frequency);
    //rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    //rcc_clock_setup_in_hsi_out_48mhz();

    //RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    //RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    //struct rcc_clock_scale rcc_hse_8mhz_external = {0};
    //rcc_hse_8mhz_external.ahb_frequency =

    const struct rcc_clock_scale rcc_hse_8mhz_external =
            { /* 84MHz */
                    .pllm = 8,
                    .plln = 336,
                    .pllp = 4,
                    .pllq = 7,
                    .pllr = 0,
                    .pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
                    .hpre = RCC_CFGR_HPRE_DIV_NONE,
                    .ppre1 = RCC_CFGR_PPRE_DIV_2,
                    .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
                    .voltage_scale = PWR_SCALE1,
                    .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
                                    FLASH_ACR_LATENCY_2WS,
                    .ahb_frequency  = 84000000,
                    .apb1_frequency = 42000000,
                    .apb2_frequency = 84000000,
            };

            //rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);

    //rcc_osc_bypass_enable(RCC_HSE);
    //rcc_osc_on(RCC_HSE);
    //rcc_wait_for_osc_ready(RCC_HSE);


    /* clock rate / 168000 to get 1mS interrupt rate */
    //systick_set_reload(168000);
    //systick_set_reload(16000);
    systick_set_reload(84000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();

	/* this done last */
	systick_interrupt_enable();
}

