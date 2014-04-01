/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			SPI (Serial Peripheral Interface (bus))
 * Author(s):		K.Andersen
 * Module:			spi.c
 * Description:	Initiates and controls the SPI
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 050128			KA			Module Created
 * 140311			NL			C Coding Standards for EMP invoked
 *
 */


/***************************** Include files *******************************/
#include "communication.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void decodeCommandTask(){
	INT8U receiveCharCMD = 0;
	INT8U temp, majorState = ST_GETCMD, minorState;
	INT8U message[] = "Error\n\r";

	INT8U command[11];
	memset(command, '\0', sizeof(command));

	int i = 0;

	while(TRUE){
		switch (majorState){
			case ST_GETCMD:
				if(receiveCharCMD == '\r'){
					receiveCharCMD = 0;
					majorState = ST_PARSECMD;
				} else if (xQueueReceive(UARTRXQueue, &receiveCharCMD, 20)){
					strcat(&command, &receiveCharCMD);
				}
				break;
			case ST_PARSECMD:

				if(strcmp(command, "reset\r") == 0){
					minorState = ST_RESET;
				} else if (strcmp(command, "start\r") == 0){
					minorState = ST_START;
				} else if (strncmp(command, "cod", 3) == 0){
					minorState = ST_COORDS;
				} else {
					majorState = ST_ERROR;
					break;
				}

				command[0] = '\0'; //Reset char array

				switch (minorState) {
					case ST_RESET:
						debug_pin(OFF);
						break;
					case ST_START:
						debug_pin(ON);
						break;
					case ST_COORDS:

						debug_pin(ON);
						break;
					default:
						break;
				}

				majorState = ST_GETCMD;
				break;
			case ST_ERROR:

				for(i = 0; i < strlen(message); i++){
					temp = message[i];
					xQueueSend(UARTTXQueue, &temp, 20);
				}

				command[0] = '\0'; //Reset char array

				majorState = ST_GETCMD;
				break;
		}
	}
}

/****************************** End Of Module *******************************/
