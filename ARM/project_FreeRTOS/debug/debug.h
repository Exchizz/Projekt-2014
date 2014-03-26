//*****************************************************************************
//
// debug.h - Macros for assisting debug of the driver library.
//
// Copyright (c) 2006-2008 Luminary Micro, Inc.  All rights reserved.
// 
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 3416 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

#include "..\uartprintf\uartprintf.h"
#include "includes\lm3s6965.h"
#include "../includes/emp_type.h"
#include "../includes/glob_def.h"
#include "../includes/binary.h"
#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG
// Debug levels
#define D_CRITICAL 	0   // Critical
#define D_ERROR 		1   // Error conditions
#define D_WARNING 	2   // Warning conditions
#define D_INFO 			3   // Informational

#define D_LEVEL 		4

// if D_LEVEL is not set from outside the source code.
// No just set the debug to error
#ifndef D_LEVEL
    #define D_LEVEL 2
#endif


#define WHERE_STR  "[file %s, line %d]: "
#define WHERE_ARG  __FILE__, __LINE__

#if(D_LEVEL > D_CRITICAL)
#define DEBUG_CRITICAL(_msg, ...) UARTprintf(WHERE_STR _msg, WHERE_ARG, __VA_ARGS__)
#else
#define DEBUG_CRITICAL(_msg, ...)
#endif

#if(D_LEVEL > D_ERROR)
#define DEBUG_ERROR(_msg, ...) UARTprintf(WHERE_STR _msg, WHERE_ARG, __VA_ARGS__)
#else
#define DEBUG_ERROR(_msg, ...)
#endif

#if(D_LEVEL > D_WARNING)
#define DEBUG_WARNING(_msg, ...) UARTprintf(WHERE_STR _msg, WHERE_ARG, __VA_ARGS__)
#else
#define DEBUG_WARNING(_msg, ...)
#endif

#if(D_LEVEL > D_INFO)
#define DEBUG_INFO(_msg, ...) UARTprintf(WHERE_STR _msg, WHERE_ARG, __VA_ARGS__)
#else
#define DEBUG_INFO(_msg, ...)
#endif

//*****************************************************************************
//
// Prototype for the function that is called when an invalid argument is passed
// to an API.  This is only used when doing a DEBUG build.
//
//*****************************************************************************
extern void __error__(char *pcFilename, unsigned long ulLine);

void debug_pin(INT8U);
void debug_pin_init();
//*****************************************************************************
//
// The ASSERT macro, which does the actual assertion checking.  Typically, this
// will be for procedure arguments.
//
//*****************************************************************************
#ifdef DEBUG
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             __error__(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif

#endif // __DEBUG_H__

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************

