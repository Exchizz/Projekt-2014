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
#include "spi.h"
#include "communication/communication_task.h"
#include "control_tasks/control_position_tilt.h"
#include "control_tasks/control_speed_tilt.h"
#include "control_tasks/control_position_pan.h"
#include "control_tasks/control_speed_pan.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Queues      *******************************/
QueueHandle_t QueuePositionPan;
QueueHandle_t QueuePositionTilt;
QueueHandle_t QueuePWMOutTilt;
QueueHandle_t QueuePWMOutPan;
QueueHandle_t QueueGoToPositionTilt;
QueueHandle_t QueueGoToPositionPan;
QueueHandle_t QueueUARTTX;
QueueHandle_t QueueUARTRX;
QueueHandle_t QueueSPITX;
QueueHandle_t QueueSPIRX;
QueueHandle_t QueuePanSpeed;
QueueHandle_t QueueTiltSpeed;
/*****************************   Functions   *******************************/
void init_alive_task(){
	//debug_pin(ON);
	// Start task
	_start(ALIVE_TASK);
	//UARTprintf("Starting up\r\n",0);

}

void alive_task(void)
{
  static INT16U time = 0;
  ++time;
  time %= 2500;
  // Toggles status LED every 1 sec.
  if (time == 0) {
    GPIO_PORTF_DATA_R ^= 0x01;
  }

  //_wait(MILLI_SEC(500));
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
	spi_init();
	gpio_init();

	/* Initialize queueHanders */
	QueuePositionPan 	= QueueCreate(1, sizeof(INT16U));
	QueuePositionTilt 	= QueueCreate(1, sizeof(INT16U));

	QueuePWMOutTilt		= QueueCreate(1, sizeof(INT16U));
	QueuePWMOutPan          = QueueCreate(1, sizeof(INT16U));

	QueueTiltSpeed		= QueueCreate(1, sizeof(INT16S));
	QueuePanSpeed           = QueueCreate(1, sizeof(INT16S));

	QueueGoToPositionTilt   = QueueCreate(1, sizeof(INT16U));
	QueueGoToPositionPan    = QueueCreate(1, sizeof(INT16U));

        QueueSPITX              = QueueCreate(2, sizeof(INT16U));
        QueueSPIRX              = QueueCreate(2, sizeof(INT16U));

        QueueUARTTX             = QueueCreate(32, sizeof(INT8U));
        QueueUARTRX             = QueueCreate(16, sizeof(INT8U));


	enable_global_int();

	start_rtcs_scheduler();		// Call scheduler

	while(1);

	return( 0 );
}

/****************************** End Of Module *******************************/
