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
#include "../includes/emp_type.h"


/*****************************    Defines    *******************************/

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

void init_spi();
/*
 * Function	:	Initiates the SPI
 */

/****************************** End Of Module *******************************/
#endif // _SPI_H
