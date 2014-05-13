/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: communication_task.h
*
* PROJECT....: G3 - Tracking system utilizing a pan/tilt system
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 1404XX  G3    Module created.
* 1405XX  G3    Module modified.
*
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
