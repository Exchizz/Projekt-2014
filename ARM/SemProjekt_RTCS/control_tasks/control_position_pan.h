/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: control_position_pan.h
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
* 140501  G3   Module created. (copy of tilt)
* 1405XX  G3   Module modified for pan. (stopband etc.)
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "application/application.h"
/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
void init_pan_position_task();
void pan_position_task();
/****************************** End Of Module *******************************/
