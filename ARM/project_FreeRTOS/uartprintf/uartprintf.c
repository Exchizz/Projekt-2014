/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: uartprintf.c
*
* PROJECT....: EMP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 100216  KHA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "uart.h"


/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

extern void uart_init(void)
/*****************************************************************************
*   Inut    : 	
*   Output   : 	
*   Function : 	
******************************************************************************/
{
	
	// Enable UART0 peripherial 
	SYSCTL_RCGC1_R  |= SYSCTL_RCGC1_UART0;
	
	/**
	* Enable the pins used by the UART
	*/
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	
	// PORTA [0:1] to alternative function
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R,0);
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R,1);
	
	// Enable digital function
	SET_BIT_HIGH(GPIO_PORTA_DEN_R,0);
	SET_BIT_HIGH(GPIO_PORTA_DEN_R,1);
	
	// Disable UART
	UART0_CTL_R &= ~(UART_CTL_UARTEN);
	
	// Clear UART pending interrupt.
	UART0_ICR_R |= 0x7F0;
	
	// Baud rate setup.
	UART0_IBRD_R = B_DIV_INT;
	UART0_FBRD_R = B_DIV_FRACT;
	
	// UART setup
	// Clear Line control register
	// UART0_LCRH_R = 0;
	// 8 bit data, 1 stop bit, no parity.
	UART0_LCRH_R |= UART_LCRH_WLEN_8;
	
	// Uart0 enable transmitter
	UART0_CTL_R |= UART_CTL_TXE;
	
	// Enable UART0
	UART0_CTL_R |= UART_CTL_UARTEN;
}

void UARTCharPut(INT8U data)
{
	if(UARTDataReady(TX))
		UART0_DR_R = data; // Send the char.
}


char UARTCharGet()
{
	return UART0_DR_R;
}

BOOLEAN UARTDataReady(INT8U direction)
{
	BOOLEAN retval = 0;
	if(direction)
		retval =  !(UART0_FR_R & UART_FR_TXFF);
	else
		retval =  !(UART0_FR_R & UART_FR_RXFE);
	return retval;
}


void UART_TX_task()
{
	INT8U dataToSend = 0;
	while(TRUE)
	{
		if(xQueueReceive(UARTTXQueue, &dataToSend, 20))
			UARTCharPut(dataToSend);
	}
}

void UART_RX_task()
{
	INT8U receivedChar = 0;
	while(TRUE)
	{
		if(xSemaphoreTake(UARTRXSem, 20) == pdTRUE){
			if(UARTDataReady(RX)){
				receivedChar = UARTCharGet();
				xQueueSend(UARTRXQueue, &receivedChar, 100);
			}
		}
	}
}
/***************************** End Of Module *******************************/












