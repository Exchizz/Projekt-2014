/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 0902012  KHA   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "communication/communication_task.h"
/*****************************    Defines    *******************************/
#define TICK_INTERUPT_MS 500
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
void init_timer0(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{
	// Enable timer peripheral clock
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
	// Disable timer
	TIMER0_CTL_R &= ~(TIMER_CTL_TAEN);
	// Enable 32-bit configuration
	TIMER0_CFG_R = 0x00000000;
	// Set to Periodic Timer mode, first clear then set
	TIMER0_TAMR_R &= ~(TIMER_TAMR_TAMR_M);
	TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
	// Set Reload value. timeout = 1sek @ F_CPU = 50 Mhz. 50000000 = 1000 ms
	TIMER0_TAILR_R = 50000*TICK_INTERUPT_MS;
	// Enable timer 0 int.
	TIMER0_IMR_R |= TIMER_IMR_TATOIM;

	// NVIC setup
	// program NVIC, vector number 35, Interrupt Number = 19
	// Clear pending interrupt
	NVIC_UNPEND0_R |= NVIC_UNPEND0_INT19;
	// Set Priority to 0x10, first clear then set.
	NVIC_PRI4_R &= ~(NVIC_PRI4_INT19_M);
	NVIC_PRI4_R |= (NVIC_PRI4_INT19_M & (0x10<<NVIC_PRI4_INT19_S));
	// enable NVIC interrupt
	NVIC_EN0_R |= NVIC_EN0_INT19;

	// Enable and start timer
	TIMER0_CTL_R |= TIMER_CTL_TAEN;

}

int last_tilt_ticks = 0;
void Timer0A_IRQHandler(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*****************************************************************************/
{

	disable_global_int();
	// Clear Timer 0 int reguest
	TIMER0_ICR_R |= TIMER_ICR_TATOCINT;
	// Toggle status led
	//GPIO_PORTF_DATA_R ^= 0x02;


	//speed_tilt = ()/0.100;
	/* PID */
	//error = SET_POINT_TILT_SPEED - speed_tilt;

	//integral += error*0.100
	//derivative = (error - previous_error)/0.100;

//	last_tilt_ticks = tilt_ticks;
//	enable_global_int();
}
void init_communication_task(){
	init_timer0();
	_start(COMMUNICATION_TASK, MILLI_SEC(0));
}
void communication_task()
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function :
*   Run @	 : 1 ms
*****************************************************************************/
{

	/*
	 * Available queues
	 * 	QueueUARTTX;
	 * 	QueueUARTRX;
	 * 	QueueSPITX;
	 * 	QueueSPIRX;
	 * */
	/* RequestSPIData(); */

	/*
	 * Test value:
	 *
	 *
	 */

	INT16U FromSPI,dataToSend = 0;
	INT16U pwmTilt = 0;

	if(QueueReceive(QueuePWMOutTilt, &pwmTilt)){
		dataToSend = (0b10 << 10) | (pwmTilt & 0x3FF);
	} else {
		dataToSend = 0;
	}

	QueueSend(QueueSPITX, &dataToSend);

	if(QueueReceive(QueueSPIRX, &FromSPI)){
		//UARTprintf("data in: %X \r\n", (FromSPI>>8));
		if((FromSPI & 0b100000000000)){//pan
			FromSPI = FromSPI & 0b011111111111;
			QueueOverwrite(QueuePositionPAN, &FromSPI);
			//UARTprintf("PAN Position: %X \t", FromSPI);
		} else {//tilt
			FromSPI = FromSPI & 0b011111111111;
			QueueOverwrite(QueuePositionTILT, &FromSPI);
			//UARTprintf("TILT Position: %X\r\n", FromSPI);
		}
	}


}
/****************************** End Of Module *******************************/
