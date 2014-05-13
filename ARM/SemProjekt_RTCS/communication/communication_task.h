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
INT8U QueuePositionPan;
INT8U QueuePositionTilt;
INT8U QueuePWMOutTilt;
INT8U QueuePWMOutPan;
/*****************************   Functions   *******************************/
void init_communication_task();
void communication_task();
/****************************** End Of Module *******************************/
