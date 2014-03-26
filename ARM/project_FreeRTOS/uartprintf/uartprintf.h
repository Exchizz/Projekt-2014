/*****************************************************************************
* Odense University College of Enginerring
* Embedded Programming (EMP)
*
* MODULENAME.: uartprintf.h
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

#ifndef _UARTPRINTF_H
  #define _UARTPRINTF_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

extern void uartprintf_init(void);
extern void UARTprintf(const char *pcString, ...);

/****************************** End Of Module *******************************/
#endif






