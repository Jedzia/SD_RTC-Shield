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
__attribute__ ((used))  void sys_tick_handler(void) {
    system_millis++;
}

/* simple sleep for delay milliseconds */
void msleep(uint32_t delay) {
    uint32_t wake = system_millis + delay;
    while(wake > system_millis);
}

/* Getter function for the current time */
uint32_t mtime(void) {
    return system_millis;
}


/*
 * clock_setup(void)
 *
 * This function sets up both the base board clock rate
 * and a 1khz "system tick" count. The SYSTICK counter is
 * a standard feature of the Cortex-M series.
 */
void sys_clock_setup(void) {
    /* Base board frequency, set to 168Mhz */
    //rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    //rcc_clock_setup_pll(&rcc_ahb_frequency);
    //rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    //rcc_clock_setup_in_hsi_out_48mhz();

    //RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    //RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    //struct rcc_clock_scale rcc_hse_8mhz_external = {0};
    //rcc_hse_8mhz_external.ahb_frequency =

//    const struct rcc_clock_scale rcc_hse_8mhz_external =
//            { /* 84MHz */
//                    .pllm = 4,
//                    .plln = 84,
//                    .pllp = 2,
//                    .pllq = 7,
//                    .pllr = 2,
//                    .pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
//                    .hpre = RCC_CFGR_HPRE_DIV_NONE,
//                    .ppre1 = RCC_CFGR_PPRE_DIV_2,
//                    .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
//                    .voltage_scale = PWR_SCALE1,
//                    .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
//                                    FLASH_ACR_LATENCY_2WS,
//                    .ahb_frequency  = 84000000,
//                    .apb1_frequency = 42000000,
//                    .apb2_frequency = 84000000,
//            };
//
//    //rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
//
#if 1
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
#else
//
//    const struct rcc_clock_scale* clock = &rcc_hse_8mhz_external;
//
//    //rcc_osc_off(RCC_HSI);
//    rcc_osc_bypass_enable(RCC_HSE);
//    //rcc_clock_setup_pll(&rcc_hse_8mhz_external);
//
//    /* Enable internal high-speed oscillator (HSI). */
//    rcc_osc_on(RCC_HSI);
//    rcc_wait_for_osc_ready(RCC_HSI);
//
//    /* Select HSI as SYSCLK source. */
//    rcc_set_sysclk_source(RCC_CFGR_SW_HSI);
//
//    /* Enable external high-speed oscillator (HSE). */
//    if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
//        rcc_osc_on(RCC_HSE);
//        //rcc_wait_for_osc_ready(RCC_HSE);
//    }
//
//    /* Set the VOS scale mode */
//    rcc_periph_clock_enable(RCC_PWR);
//    pwr_set_vos_scale(clock->voltage_scale);
//
//    /*
//     * Set prescalers for AHB, ADC, ABP1, ABP2.
//     * Do this before touching the PLL (TODO: why?).
//     */
//    rcc_set_hpre(clock->hpre);
//    rcc_set_ppre1(clock->ppre1);
//    rcc_set_ppre2(clock->ppre2);
//
//    /* Disable PLL oscillator before changing its configuration. */
//    rcc_osc_off(RCC_PLL);
//
//    /* Configure the PLL oscillator. */
//    if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
//        rcc_set_main_pll_hse(clock->pllm, clock->plln,
//                clock->pllp, clock->pllq, clock->pllr);
//    } else {
//        rcc_set_main_pll_hsi(clock->pllm, clock->plln,
//                clock->pllp, clock->pllq, clock->pllr);
//    }
//
//    /* Enable PLL oscillator and wait for it to stabilize. */
//    rcc_osc_on(RCC_PLL);
//    //rcc_wait_for_osc_ready(RCC_PLL);
//
//    /* Configure flash settings. */
//    if (clock->flash_config & FLASH_ACR_DCEN) {
//        flash_dcache_enable();
//    } else {
//        flash_dcache_disable();
//    }
//    if (clock->flash_config & FLASH_ACR_ICEN) {
//        flash_icache_enable();
//    } else {
//        flash_icache_disable();
//    }
//    flash_set_ws(clock->flash_config);
//
//    /* Select PLL as SYSCLK source. */
//    rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
//
//    /* Wait for PLL clock to be selected. */
//    //rcc_wait_for_sysclk_status(RCC_PLL);
//
//    /* Set the peripheral clock frequencies used. */
//    rcc_ahb_frequency  = clock->ahb_frequency;
//    rcc_apb1_frequency = clock->apb1_frequency;
//    rcc_apb2_frequency = clock->apb2_frequency;
//
//    /* Disable internal high-speed oscillator. */
//    if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
//        rcc_osc_off(RCC_HSI);
//    }
//
//
//
//
//    //rcc_osc_on(RCC_HSE);
//    //rcc_wait_for_osc_ready(RCC_HSE);
//#endif


//      *** From CubeMX ***
//    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//    /** Configure the main internal regulator output voltage
//    */
//    __HAL_RCC_PWR_CLK_ENABLE();
//    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//    RCC_OscInitStruct.PLL.PLLM = 4;
//    RCC_OscInitStruct.PLL.PLLN = 84;
//    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//    RCC_OscInitStruct.PLL.PLLQ = 7;
//    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//    {
//        Error_Handler();
//    }
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//    {
//        Error_Handler();
//    }
#endif






    /* clock rate / 168000 to get 1mS interrupt rate */
    //systick_set_reload(168000);
    //systick_set_reload(16000);
    systick_set_reload(84000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();

    /* this done last */
    systick_interrupt_enable();
}

