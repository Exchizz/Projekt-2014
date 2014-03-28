/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: uart.h
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
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _UART_H
  #define _UART_H

/***************************** Include files *******************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "include/semphr.h"

#include "..\cpu\cpu.h"
#include <stdarg.h>
#include "../includes/lm3s6965.h"
#include "../includes/emp_type.h"
#include "../includes/glob_def.h"
#include "../includes/binary.h"

/*****************************    Defines    *******************************/
// FCPU are defined in cpu.h
#define BAUD_RATE   						115200	  // Baud rate [bits/s]
#define UARTSYSCLK							FCPU			// 

#if	defined(BAUD_RATE) && defined(UARTSYSCLK)

// Calculate baud rate divisor
#define B_DIV_TEMP	 	( (UARTSYSCLK /BAUD_RATE)<<2)
#define B_DIV_INT			B_DIV_TEMP>>6

#if ((B_DIV_INT >= 1) && (B_DIV_INT <= 0xFFFF))
	// Integer part within range

	// Calculate fraction part
	#define B_DIV_FRACT 	(0x0003F & B_DIV_TEMP)
	

#else
	// Integer part outside range
	// Baud rate divisor error
	#if (B_DIV_INT < 1)
		#error	Baud rate to high
	#endif

	#if (B_DIV_INT > 0xFFFF)
		#error	Baud rate to low
	#endif
#endif
#else
	#error "Baud-rate or UARTSYSCLK not defined" 
#endif

#define RX	0
#define	TX	1
/*****************************   Constants   *******************************/

xQueueHandle UARTTXQueue, UARTRXQueue;

xSemaphoreHandle UARTRXSem;

/*****************************   Functions   *******************************/

extern void uart_init(void);
extern void UARTprintf(const char *pcString, ...);

void UART_TX_task();
void UART_RX_task();

BOOLEAN UARTDataReady(INT8U);


/****************************** End Of Module *******************************/
#endif






