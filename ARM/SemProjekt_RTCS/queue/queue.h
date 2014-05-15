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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc\emp_type.h"
#include "inc\glob_def.h"
/*****************************    Defines    *******************************/
typedef struct {
        INT8U wr;
        INT8U rd;
        INT8U typesize;
        INT8U queuesize;
        int* mem;
        INT8U elements;
        INT8U overwrite;
        INT8U id;
} QueueHandle_t;
#define DEBUG_ON	1
#define DEBUG_OFF	0
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
QueueHandle_t QueueCreate(INT8U queuesize, INT8U typesize);
void QueueOverwrite(QueueHandle_t *this, const void * dataIn);
BOOLEAN QueueSend(QueueHandle_t *this, const void * dataIn);
BOOLEAN QueueSend(QueueHandle_t *this, const void * dataIn);
INT8U QueueSpaceLeft(QueueHandle_t *this);
BOOLEAN QueuePeek(QueueHandle_t *this, void * dataOut);
/*****************************   Functions   *******************************/
/****************************** End Of Module *******************************/
