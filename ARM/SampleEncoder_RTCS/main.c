/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: G3 - Tracking system utilizing a pan/tilt system
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090212  KHA   Module created.
* 1405XX  G3    Module drastically changed to fit project.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "inc\lm3s6965.h"
#include "inc\emp_type.h"
#include "cpu\cpu.h"
#include "rtcs\rtcs.h"
#include "debug/debug.h"
#include "uartprintf.h"
#include "queue.h"
#include "GPIO/gpio.h"
#include "communication/communication_task.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
INT32U encoderTicks = 0x80000000;
/*****************************   Variables   *******************************/
/*****************************   Queues      *******************************/
QueueHandle_t QueueUARTTX;
QueueHandle_t QueueUARTRX;

/*****************************   Functions   *******************************/
void init_alive_task(){
	//debug_pin(ON);
	// Start task
	_start(ALIVE_TASK, MILLI_SEC(500));
	//UARTprintf("Starting up\r\n",0);

}

void alive_task(void)
{
	// Toggles status LED every 1 sec.
	GPIO_PORTF_DATA_R ^= 0x01;
	_wait(MILLI_SEC(500));
}





int main(void)
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
	disable_global_int();
	clk_system_init();
	uartprintf_init();
	gpio_init();

        QueueUARTTX             = QueueCreate(16, sizeof(INT8U));
        QueueUARTRX             = QueueCreate(16, sizeof(INT8U));


	enable_global_int();

	volatile INT32U var;
	for (var = 0; var < 0xFFFFFE; ++var);

	start_rtcs_scheduler();		// Call scheduler

	while(1);

	return( 0 );
}

/****************************** End Of Module *******************************/
