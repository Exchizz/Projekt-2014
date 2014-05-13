/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.:converter.h
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
* 140511  G3   Module created.
* 1405XX  G3   Module modified.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "queue.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
INT8U QueueGoToPositionTilt;
INT8U QueueGoToPositionPan;

/*****************************   Functions   *******************************/
void init_converter_task();
void converter_task();
/****************************** End Of Module *******************************/
