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

	INT8U command[6] = {0, 0, 0, 0, 0, 0};
void resetCharArray(char * data, int length){
	int i;
	for(i = 0; i < length; i++)
		data[i] = 0;
}
void decodeCommandTask(){
	INT8U receiveCharCMD = 0;
	//INT8U tmp[6] = {'r','e','s','e','t','\0'};
	INT8U tmp[6] = "reset";
	INT8U temp;
	int i = 0;

	while(TRUE){

		if(xQueueReceive(UARTRXQueue, &receiveCharCMD, 20)){
			if(receiveCharCMD == '\r'){
				//Full command received.
				if(strcmp(command, "reset") == 0){
					debug_pin(ON);
					//strcpy(command, "");
					resetCharArray(command, strlen(command));

				}
				if(strcmp(command, "lukas") == 0){
					debug_pin(OFF);
					//strcpy(command, "");
					resetCharArray(command, strlen(command));
				}
					for(i = 0; i < strlen(command); i++){
						temp = command[i];
						xQueueSend(UARTTXQueue, &temp, 20);
					}
			} else {
				if(strlen(command) >= 6){
					//resetCharArray(command, strlen(command));
					//debug_pin(ON);
					INT8U message[] = "Error\n\r";
					for(i = 0; i < strlen(message); i++){
						temp = message[i];
						xQueueSend(UARTTXQueue, &temp, 20);
					}
					resetCharArray(command, strlen(command));
					for(i = 0; i < 7; i++);
//					for(i = 0; i < strlen(command); i++){
//						temp = command[i];
					//	xQueueSend(UARTTXQueue, &temp, 20);
					//}
				} else{
					strcat(&command, &receiveCharCMD);
				}
			}
		}
	}
}

/****************************** End Of Module *******************************/
