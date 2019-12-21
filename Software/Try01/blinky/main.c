//#include "api.h"
//#include "api-asm.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void)
{
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

static void usart_setup(void)
{
	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

static void gpio_setup(void)
{
	/* Set GPIO5 (in GPIO port A) to 'output push-pull'. */
	/* Manually: */
	/*GPIOA_CRH = (GPIO_CNF_OUTPUT_PUSHPULL << (((8 - 8) * 4) + 2)); */
	/*GPIOA_CRH |= (GPIO_MODE_OUTPUT_2_MHZ << ((8 - 8) * 4)); */
	/* Using API functions: */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

int main(void) {
	
	int i, j = 0, c = 0;

	clock_setup();
	gpio_setup();
	usart_setup();

	/* Blink the LED (PC8) on the board. */
	while (1) {
		/* Manually: */
		/* GPIOA_BSRR = GPIO5; */		/* LED off */
		/* for (i = 0; i < 1000000; i++) */	/* Wait a bit. */
		/*	__asm__("nop"); */
		/* GPIOA_BRR = GPIO5; */		/* LED on */
		/* for (i = 0; i < 1000000; i++) */	/* Wait a bit. */
		/*	__asm__("nop"); */

		/* Using API functions gpio_set()/gpio_clear(): */
		/* gpio_set(GPIOA, GPIO5); */	/* LED off */
		/* for (i = 0; i < 1000000; i++) */	/* Wait a bit. */
		/*	__asm__("nop"); */
		/* gpio_clear(GPIOA, GPIO5); */	/* LED on */
		/* for (i = 0; i < 1000000; i++) */	/* Wait a bit. */
		/*	__asm__("nop"); */

		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIOA, GPIO5);	/* LED on/off */

		usart_send_blocking(USART2, c + '0'); /* USART2: Send byte. */
		c = (c == 9) ? 0 : c + 1;	/* Increment c. */
		if ((j++ % 80) == 0) {		/* Newline after line full. */
			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');
		}

		for (i = 0; i < 1500000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}

		gpio_toggle(GPIOB, GPIO3);	/* LED on/off */
		for (i = 0; i < 250000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
		
		gpio_toggle(GPIOB, GPIO5);	/* LED on/off */
		for (i = 0; i < 50000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
}

	return 0;
	
	/* add your own code */
	uint32_t rev = 0xaabbccdd;
	rev = rev_bytes(rev);
	return my_func(rev);
}
