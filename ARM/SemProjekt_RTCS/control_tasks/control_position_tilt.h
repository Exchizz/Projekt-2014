/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: control_position_tilt.h
*
* PROJECT....: G3 - Tracking system utilizing a pan/tilt system
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 140501  G3   Module created.
* 1405XX  G3   Module modified
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "communication/communication_task.h"
#include "Converter/converter.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
INT8U QueueTiltSpeed;
/*****************************   Functions   *******************************/
void init_tilt_position_task();
void tilt_position_task();
/****************************** End Of Module *******************************/
