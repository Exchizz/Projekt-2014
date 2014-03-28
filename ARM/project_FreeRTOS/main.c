/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: main.c
*
* PROJECT....: FreeRTOS adaption.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140324  MoH	Module created.
*
*****************************************************************************/


/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "include/semphr.h"

#include "emp_type.h"
#include "includes/glob_def.h"
#include "binary.h"
#include "debug/debug.h"
#include "uartprintf/uart.h"
#include "spi/spi.h"

#include "communication.h"

/*****************************    Defines    *******************************/
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO	0
#define LOW_PRIO	1
#define	MED_PRIO	2
#define	HIGH_PRIO 	3

void status_led_init(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
  INT8S dummy;
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output (PF0).
  SET_BIT_HIGH(GPIO_PORTF_DIR_R, PF0);

  // Enable the GPIO pins for digital function (PF0).
  SET_BIT_HIGH(GPIO_PORTF_DEN_R, PF0);

  // Set pin high
  SET_BIT_HIGH(GPIO_PORTF_DATA_R, PF0);
}


void status_led_task(void *pvParameters)
{

	status_led_init();
	//long int i;
	//INT8U test = 0x30;
	while(TRUE)
	{
		//test++;
		//xQueueSend(SPITXQueue, &test, 20);
		TOGGLE_BIT(GPIO_PORTF_DATA_R, PF0);
		vTaskDelay(500 / portTICK_RATE_MS); // wait 100 ms.
	}
}

void poleRegs()
{
	while(TRUE)
	{
		if(UARTDataReady(RX)){
			xSemaphoreGive(UARTRXSem);
		}
		if(spiDataReady(RX)){
			xSemaphoreGive(SPIRXSem);
		}
		vTaskDelay(5 / portTICK_RATE_MS);
	}
}

int main(void)
{
	portBASE_TYPE return_value = pdTRUE;

	clk_system_init();

	debug_pin_init();
	debug_pin(OFF);

	uart_init();
	spi_init();

	/*
	* Start the tasks defined within this file/specific to this demo.
	*/

	UARTRXQueue = xQueueCreate(1, sizeof(INT8U)); //Receive queue
	UARTTXQueue = xQueueCreate(1, sizeof(INT8U)); //Transmit queue

	SPIRXQueue = xQueueCreate(1, sizeof(INT16U));
	SPITXQueue = xQueueCreate(1, sizeof(INT16U));

	vSemaphoreCreateBinary(UARTRXSem);
	xSemaphoreTake(UARTRXSem, 20);
	vSemaphoreCreateBinary(SPIRXSem);
	xSemaphoreTake(SPIRXSem, 20);

	return_value &= xTaskCreate( status_led_task, ( signed portCHAR * ) "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( UART_TX_task, ( signed portCHAR * ) "transmit_Task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( UART_RX_task, ( signed portCHAR * ) "receive_Task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

	xTaskCreate( poleRegs, ( signed portCHAR * ) "pole_register_task", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

	xTaskCreate( spiRXTask, ( signed portCHAR * ) "spiReceiveTask", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
	xTaskCreate( spiTXTask, ( signed portCHAR * ) "spiTransmitTask", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

	xTaskCreate( decodeCommandTask, ( signed portCHAR * ) "ParseCommands", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

	/*
	* Start the scheduler.
	*/
	vTaskStartScheduler();

	/*
	* Will only get here if there was insufficient memory to create the idle task.
	*/
	return 1;

}
