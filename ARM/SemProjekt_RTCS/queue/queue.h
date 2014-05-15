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
        int wr;
        int rd;
        int typesize;
        int queuesize;
        char* mem;
        int elements;
        int overwrite;
} QueueHandle_t;
#define DEBUG_ON	1
#define DEBUG_OFF	0
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
QueueHandle_t QueueCreate(int queuesize, int typesize);
void QueueOverwrite(QueueHandle_t *this, const void * dataIn);
BOOLEAN QueueSend(QueueHandle_t *this, const void * dataIn);
BOOLEAN QueueSend(QueueHandle_t *this, const void * dataIn);
INT8U QueueSpaceLeft(QueueHandle_t *this);
BOOLEAN QueuePeek(QueueHandle_t *this, void * dataOut);
/*****************************   Functions   *******************************/
/****************************** End Of Module *******************************/
