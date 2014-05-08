/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: ECP
*
* DESCRIPTION:
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
#include "queue.h"
#include "debug/debug.h"
#include "spi.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
INT8U QueuePositionPAN;
INT8U QueuePositionTILT;
INT8U QueuePWMOutTilt;
/*****************************   Functions   *******************************/
void init_controller_task();
void controller_task();
/****************************** End Of Module *******************************/
