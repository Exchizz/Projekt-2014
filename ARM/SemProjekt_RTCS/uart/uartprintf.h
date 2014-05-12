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
INT8U QueueUARTTX;
INT8U QueueUARTRX;

#define RX	0
#define TX	1
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void init_uart_rx_task();
void init_uart_tx_task();
void UART_RX_task();
void UART_tX_task();
extern void uartprintf_init(void);
extern void UARTprintf(const char *pcString, ...);

/****************************** End Of Module *******************************/
#endif






