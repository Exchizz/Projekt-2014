/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			EMP
 * Author(s):		M.Hansen
 * Module:			gpio.c
 * Description:
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 090215			MoH		Module Created
 * 140311			NL			C Coding Standards for EMP invoked
 *
 */


/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_gpio(void)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
  INT8S dummy;

  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOB | SYSCTL_RCGC2_GPIOF;

  // digital enable a and b
  GPIO_PORTA_DEN_R |= 0b11000000;
  GPIO_PORTB_DEN_R |= 0b01111100;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // enable port A6-7
  GPIO_PORTA_DIR_R |= 0b11000000;

  // enable port B2-6
  GPIO_PORTB_DIR_R |= 0b01111100; // output

  // Set the direction as output PF1
  GPIO_PORTF_DEN_R |= 0x02;

  // Enable internal pull-up (PF1).
  GPIO_PORTF_PUR_R |= 0x02;
}


/****************************** End Of Module *******************************/
