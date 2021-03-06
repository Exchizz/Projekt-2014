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
#include "spi/spi.h"
/*****************************    Defines    *******************************/

#define ST_GETCMD 	0
#define ST_PARSECMD	1
#define ST_RESET	2
#define ST_START	3
#define ST_ERROR	4
#define ST_COORDS	5

#define RESET		0
#define ERROR		1
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
	void decodeCommandTask();

/****************************** End Of Module *******************************/
#endif // _SPI_H
