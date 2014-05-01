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
			command[20] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'},
			codX[3],codY[3],
			subState,
			cmdError = FALSE;
			commStat;
			char receiveCharCMD = 0;

void decodeCommandTask(){

	while(TRUE){
		switch (state){
			case ST_GETCMD:

				if(receiveCharCMD == '\r' || (strlen(command) >= (sizeof(command)))){
					receiveCharCMD = 0;
					state = ST_PARSECMD;
				} else if (xQueueReceive(UARTRXQueue, &receiveCharCMD, 20)){
					strcat(command, &receiveCharCMD);

				}

				break;
			case ST_PARSECMD:
				if(strncmp(command,"cod",3)==0){
					subState = ST_COORDS;
				} else if (strcmp(command, "start\r") == 0){
					subState = ST_START;
				} else if (strcmp(command, "reset\r") == 0) {
					subState = ST_RESET;
				} else {
					cmdError = TRUE;
				}

				switch(subState){
					case ST_START:
						debug_pin(ON);
						break;
					case ST_RESET:
						debug_pin(OFF);
						commStat = RESET;
						xQueueSend(SPITXQueue, &commStat, 20);
						break;
					case ST_COORDS:
						strncpy(codX,command+4,3);

						i = (100*(codX[0]-'0') + 10*(codX[1]-'0') + codX[2]-'0');
						xQueueSend(UARTTXQueue, &i, 20);

						strncpy(codY,command+8,3);

						i = (100*(codY[0]-'0') + 10*(codY[1]-'0') + codY[2]-'0');
						xQueueSend(UARTTXQueue, &i, 20);
						break;
					default:
						break;
				}

				strcpy(command, "");

				if (cmdError){
					cmdError = FALSE;
					state = ST_ERROR;
				} else {
					state = ST_GETCMD;
				}

				break;
			case ST_ERROR:
				//strcpy(command, "ERROR!\n\r");

				//for (i = 0; i < strlen(command); i++) {
					//xQueueSend(UARTTXQueue, &command[i], 20);
				//}
				commStat = ERROR;
				xQueueSend(UARTTXQueue, &commStat, 20);
				strcpy(command, "");

				state = ST_GETCMD;
				break;
		}
	}
}

/****************************** End Of Module *******************************/
