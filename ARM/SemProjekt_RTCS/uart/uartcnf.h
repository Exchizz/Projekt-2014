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
#include "cpu/cpu.h"

/*****************************    Defines    *******************************/
// FCPU are defined in cpu.h
#define BAUD_RATE   						3125000	  // Baud rate [bits/s]
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

//	// Enable UART0
//	SYSCTL_RCGC1_R  |= SYSCTL_RCGC1_UART0;  
//	
//	UART0_IBRD_R = B_DIV_INT;
//	UART0_FBRD_R = B_DIV_FRACT;
//
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif






