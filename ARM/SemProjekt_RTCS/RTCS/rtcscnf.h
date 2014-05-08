/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
* 
* MODULENAME.: rtcscnf.h
*
* PROJECT....: EMP
*
* DESCRIPTION: The RTCS configuration file.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _RTCSCNF_H
  #define _RTCSCNF_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

// Definition of RTCS timer frequency in Hz.
#define RTCS_TIMER_FREQ					50000000

// Definition of system tick in millisec.
#define T_TICK						1

// Definition of user tasks. Always start from task 0 and proceed onwards.
// If no task is required, write NULL. LAST_TASK must be set to the task
// number of the last task.
//
// Note : o Tasks are _always_ numbered from 0 and onwards.
//        o Tasks and initialisation functions are declared as external
//          automatically and assumed to be of type 'void Task(void)'.

// Task number of LAST task
#define LAST_TASK					6

#define TASK0         		alive_task
#define TASK1         		UART_TX_task
#define TASK2         		UART_RX_task
#define TASK3         		controller_task
#define TASK4         		SPI_RX_task
#define TASK5         		SPI_TX_task
#define TASK6         		tilt_control_task
#define TASK7         		NULL

#define INIT_TASK0   		init_alive_task
#define INIT_TASK1      	init_uart_tx_task
#define INIT_TASK2      	init_uart_rx_task
#define INIT_TASK3      	init_controller_task
#define INIT_TASK4      	init_SPI_RX_task
#define INIT_TASK5      	init_SPI_TX_task
#define INIT_TASK6      	init_tilt_control_task
#define INIT_TASK7      	NULL

//
// Definition of task names. For customization, change the symbol
// immediately AFTER the #define, e.g.
//
//      #define MY_TASK   0

#define ALIVE_TASK				0
#define UART_TX_TASK			1
#define UART_RX_TASK			2
#define CONTROLLER_TASK			3
#define SPI_RX_TASK				4
#define SPI_TX_TASK				5
#define TILT_CONTROL_TASK		6
#define TASK7_NAME				7


//
// Definition of semaphore names. For customization, change the symbol
// immediately AFTER the #define, e.g.
//
//      #define MY_SEMAPHORE    BIT7

#define SEM0								BIT0
#define SEM1								BIT1
#define SEM2								BIT2
#define SEM3								BIT3
#define SEM4        						BIT4
#define SEM5        						BIT5
#define SEM6        						BIT6
#define SEM7        						BIT7
#define SEM8        						BIT8
#define SEM9        						BIT9
#define SEM10        						BIT10
#define SEM11        						BIT11
#define SEM12        						BIT12
#define SEM13        						BIT13
#define SEM14        						BIT14
#define SEM15        						BIT15


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
