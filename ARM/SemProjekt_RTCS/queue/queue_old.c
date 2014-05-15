/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: G3 - Tracking system utilizing a pan/tilt system
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

/***************************** Include files *******************************/
#include "queue.h"

/*****************************    Defines    *******************************/
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
INT8U CreateQueueHandle(){
	static INT8U QueueCounter = 0;
	Queue[QueueCounter].rw = EMPTY;
	return QueueCounter++;
}
void QueueSend(INT8U QueueHandle, INT16U *data){
	if(QueueHandle < QUEUESIZE){
		if(Queue[QueueHandle].rw == EMPTY){
			Queue[QueueHandle].data = *data;
			Queue[QueueHandle].rw = FULL;
		}
	}
}


BOOLEAN QueuePeek(INT8U QueueHandle, INT16U * dataOut){
	INT8U retval = FALSE;
	if(Queue[QueueHandle].rw == FULL){
		*dataOut = Queue[QueueHandle].data;
		retval = TRUE;
	}
	return retval;
}

void QueueOverwrite(INT8U QueueHandle, INT16U* data){
	if(QueueHandle < QUEUESIZE){
		Queue[QueueHandle].data = *data;
		Queue[QueueHandle].rw = FULL;
	}
}
BOOLEAN QueueEmpty(INT8U QueueHandle){
	INT8U retval = FALSE;
	if(Queue[QueueHandle].rw == EMPTY){
		retval = TRUE;
	}
	return retval;
}
BOOLEAN QueueReceive(INT8U QueueHandle, INT16U* dataOut){
	BOOLEAN retval = FALSE;
	if(Queue[QueueHandle].rw == FULL){
		*dataOut = Queue[QueueHandle].data;
		Queue[QueueHandle].rw = EMPTY;
		retval = TRUE;
	}
	return retval;
}
/****************************** End Of Module *******************************/
