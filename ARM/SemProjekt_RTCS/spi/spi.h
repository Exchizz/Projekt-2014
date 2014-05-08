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

#ifndef _SPI_H
#define _SPI_H

/***************************** Include files *******************************/
#include "inc/emp_type.h"
#include "inc/lm3s6965.h"
#include "queue/queue.h"

/*****************************    Defines    *******************************/
#define RX	0
#define TX	1

INT8U QueueSPITX;
INT8U QueueSPIRX;
/*****************************   Constants   *******************************/
/*****************************   Functions   *******************************/

void spi_enable( void );
/*
 * Function	:	Enables SPI
 */

void spi_disable( void );
/*
 * Function	:	Disables SPI
 */

void spi_init();
/*
 * Function	:	Initiates the SPI
 */

void spiTXTask();

void spiRXTask();

BOOLEAN spiDataReady(INT8U);
/****************************** End Of Module *******************************/
#endif // _SPI_H
