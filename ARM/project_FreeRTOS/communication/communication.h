/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			SPI (Serial Peripheral Interface (bus))
 * Author(s):		K.Andersen
 * Module:			spi.h
 * Description:	Initiates and controls the SPI
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 050128			KA			Module Created
 * 140311			NL			C Coding Standards for EMP invoked
 *
 */

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

/***************************** Include files *******************************/
#include "../includes/emp_type.h"
#include "../includes/lm3s6965.h"
#include "../includes/binary.h"
#include "../debug/debug.h"

#include <string.h>
#include <stdio.h>


#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
	void decodeCommandTask();

/****************************** End Of Module *******************************/
#endif // _SPI_H
