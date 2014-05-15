#include "queue/queue.h"

#define DEBUG DEBUG_OFF

QueueHandle_t QueueCreate(int queuesize, int typesize){
	QueueHandle_t currentQueue;
	currentQueue.mem = calloc(queuesize, typesize);
	if(!currentQueue.mem){
		UARTprintf("Unable to allocate memory \r\n");
	}
	currentQueue.wr = 0;
	currentQueue.rd = 0;
	currentQueue.typesize = typesize;
	currentQueue.queuesize = queuesize;
	currentQueue.elements = 0;
	currentQueue.overwrite = FALSE;
	return currentQueue;
}

void QueueOverwrite(QueueHandle_t *this, const void * dataIn){
	//Write-pointer overrun
	this->wr %=this->queuesize;

	//++this->elements;
#if DEBUG == 1
	UARTprintf("QueueOverwrite: elements: %d, queueSize: %d \r\n", this->elements, this->queuesize);
#endif
	if(this->elements < this->queuesize){
		++this->elements;
	} else {
#if DEBUG == 1
	UARTprintf("\t overwrite\r\n");
#endif
		this->overwrite = TRUE;
	}

	//Copy content of void * dataIn to the queue, and increment write-pointer
	memcpy ( this->mem+((this->typesize*this->wr-1)%this->queuesize), dataIn, this->typesize );
}

BOOLEAN QueueSend(QueueHandle_t *this, const void * dataIn){
	BOOLEAN retval = FALSE;

	//Write-pointer overrun
	this->wr %= this->queuesize;
#if DEBUG == 1
	UARTprintf("QueueSend: Elements: %d, writepointer: %d \r\n", this->elements, this->wr);
#endif
	if(this->overwrite){
		this->rd = this->wr;
		this->overwrite = FALSE;
	}
	if(this->elements < this->queuesize){
		++this->elements;
		//Copy content of void * dataIn to the queue, and increment write-pointer
		memcpy ( this->mem+(this->typesize*(this->wr++)), dataIn, this->typesize );
		retval = TRUE;
	}
#if DEBUG == 1
	UARTprintf("\t after increase: %d \r\n", this->wr);
#endif
	return retval;
}

BOOLEAN QueueReceive(QueueHandle_t *this, void * dataOut){
	BOOLEAN retval = FALSE;

#if DEBUG == 1
	UARTprintf("QueueReceive: Elements: %d, readpointer: %d \r\n", this->elements, this->rd);
#endif
	//check if data is available
	if(this->elements > 0){
		--this->elements;
		//Copy content of queue to dataOut
		memcpy( dataOut, this->mem+(this->typesize*this->rd++), this->typesize);
		//Pointer read overrun
		this->rd %= this->queuesize;
		retval = TRUE;
	}
	return retval;
}

int QueueSpaceLeft(QueueHandle_t *this){
	return (this->queuesize)-(this->elements);
}

BOOLEAN QueuePeek(QueueHandle_t *this, void * dataOut){
	BOOLEAN retval = FALSE;

	//check if data is available
	if(this->elements > 0){
		//Copy content of queue to dataOut
		memcpy( dataOut, this->mem+(this->typesize*this->rd), this->typesize);
		retval = TRUE;
	}
	return retval;
}


/*
int main(){

	QueueHandle_t Queue1 = QueueCreate(8, sizeof(char));
	char dataOut;

	char dataIn = 'A';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'B';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'C';
	QueueOverwrite(&Queue1,&dataIn);
	dataIn = 'D';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'E';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'F';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'G';
	QueueSend(&Queue1, &dataIn);
	dataIn = 'H';
	QueueSend(&Queue1, &dataIn);

	dataIn = 'I';
	QueueOverwrite(&Queue1, &dataIn);
	dataIn = 'J';
	QueueOverwrite(&Queue1, &dataIn);

	int i = 0;
	for(; i < 10; i++){
		if(QueueReceive(&Queue1, &dataOut)){
			uartprintf("%d: Out from queue: %c \r\n",i ,dataOut);
		} else {
			uartprintf("No data available\r\n");
		}
	}

	return 1;
}
*/
