/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 0902012  KHA   Module created.
*
*****************************************************************************/
#pragma once
/***************************** Include files *******************************/
#include "inc\lm3s6965.h"
#include "inc\emp_type.h"
#include "cpu\cpu.h"
#include "rtcs\rtcs.h"
#include "uartprintf.h"
#include "stdlib.h"
#include "inc\glob_def.h"
/*****************************    Defines    *******************************/
#define QUEUESIZE	10
#define EMPTY		1
#define FULL 		2
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
struct QueueData {
	INT16U data;
	BOOLEAN rw;
};

struct QueueData Queue[QUEUESIZE];
/*****************************   Functions   *******************************/
BOOLEAN QueueEmpty(INT8U QueueHandle);
INT8U CreateQueueHandle();
void QueueSend(INT8U QueueHandle, INT16U * data);
BOOLEAN QueueReceive(INT8U QueueHandle, INT16U* dataOut);
void QueueOverwrite(INT8U QueueHandle, INT16U * data);
/****************************** End Of Module *******************************/
