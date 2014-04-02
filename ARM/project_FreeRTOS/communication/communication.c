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
	INT8U 	i = 0,
			state = ST_GETCMD,
			command[20],
			codX[3],codY[3],
			subState,
			cmdError = FALSE;
			char receiveCharCMD = 0;

void decodeCommandTask(){


	strcpy((char *)command, "");

	while(TRUE){
		switch (state){
			case ST_GETCMD:

				if(receiveCharCMD == '\r' || (strlen((char *)command) >= (sizeof((char *)command)))){
					receiveCharCMD = 0;
					state = ST_PARSECMD;
				} else if (xQueueReceive(UARTRXQueue, &receiveCharCMD, 20)){
					strcat((char)command, &receiveCharCMD);

				}

				break;
			case ST_PARSECMD:

				//for (i = 0; i < strlen(command); i++) {
					//xQueueSend(UARTTXQueue, &command[i], 20);
				//}

				//debug_pin(ON);
				if(strncmp((char*)command,"cod",3)==0){
					subState = ST_COORDS;
				}  else if (strcmp((char *)command, "start\r") == 0){
					subState = ST_START;
					//debug_pin(ON);
				} else if (strcmp((char *)command, "reset\r") == 0) {
					subState = ST_RESET;
					//debug_pin(OFF);
				} else {
					cmdError = TRUE;
				}

				switch(subState){
					case ST_START:
						debug_pin(ON);
						break;
					case ST_RESET:
						debug_pin(OFF);
						break;
					case ST_COORDS:
						strncpy((char *)codX,(char *)command+4,3);

						i = (100*(codX[0]-'0') + 10*(codX[1]-'0') + codX[2]-'0');
						xQueueSend(UARTTXQueue, &i, 20);

						strncpy((char*)codY,(char*)command+8,3);

						i = (100*(codY[0]-'0') + 10*(codY[1]-'0') + codY[2]-'0');
						xQueueSend(UARTTXQueue, &i, 20);
						debug_pin(ON);
						break;
					default:
						break;
				}

				strcpy((char*)command, "");

				if (cmdError){
					cmdError = FALSE;
					state = ST_ERROR;
				} else {
					state = ST_GETCMD;
				}

				break;
			case ST_ERROR:
				strcpy((char*)command, "ERROR!\n\r");

				for (i = 0; i < strlen((char*)command); i++) {
					xQueueSend(UARTTXQueue, &command[i], 20);
				}

				strcpy((char*)command, "");

				state = ST_GETCMD;
				break;
		}
	}
}

/****************************** End Of Module *******************************/
