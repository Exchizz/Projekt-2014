/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			SPI (Serial Peripheral Interface (bus))
 * Author(s):		N.Lynnerup
 * Module:			main.c
 * Description:	Sends data through an SPI protocol
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 140311			NL			Module Created
 * 								C Coding Standards for EMP invoked
 *
 */

/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"
#include "gpio.h"
#include "systick.h"
#include "swtimers.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT16S alive_timer = TIM_500_MSEC;
int counter = 0;


int main(void)
{
	disable_global_int();
	init_clk_system();
	init_gpio();
	init_status_led();
	init_spi();
	systick_init();
	enable_global_int();

	while(1)
	{
		/* System part of the super loop. */
		while(!ticks);
		// The following will be executed every 5mS
		ticks--;
		SSI0_DR_R = 0xFFFF;		// Send this through SPI
		if (!--alive_timer)
		{
			alive_timer = TIM_500_MSEC;
			GPIO_PORTF_DATA_R ^= 0x01;
		}
		/* Application part of the super loop. */
		//counter = SSI0_SR_R;
	}
	return (0);
}
