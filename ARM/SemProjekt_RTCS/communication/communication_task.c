/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: communication_task.c
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
 * 1404XX  G3    Communication task added.
 * 1405XX  G3    Communication task modified.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "communication/communication_task.h"
/*****************************    Defines    *******************************/
#define TICK_INTERUPT_MS 500

#define NORMAL 0
#define DEBUGINFO 1
#define RUNMODE NORMAL

#define TILTMESSAGE_SENDPWM 0b01
#define TILTMESSAGE_SENDPWM 0b10
#define MASK_DIRECITION_PWM 0x3FF
#define MASK_MESSAGE_POSITION 0b011111111111
#define MASK_MESSAGERECEIVE_MOTOR 0b100000000000

/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
void init_communication_task(){
  _start(COMMUNICATION_TASK, MILLI_SEC(0));
}
void communication_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function : send data (pwm) to FPGA via SPI and receive position and put in matching queue
 *   Run @	 : 1 ms
 *****************************************************************************/
{

  INT16U FromSPI,dataToSend = 0;
  INT16U pwmTilt = 0, pwmPan = 0;

  // get data from pan/tilt queue and put in data to send var
  if(QueueReceive(QueuePWMOutTilt, &pwmTilt)){
    dataToSend = (TILTMESSAGE_SENDPWM << 10) | (pwmTilt & MASK_DIRECITION_PWM);
  }
  else if (QueueReceive(QueuePWMOutPan, &pwmPan)) {
    dataToSend = (PANMESSAGE_SENDPWM << 10) | (pwmPan & MASK_DIRECITION_PWM);
  }
  else {
    // if no pwm to send, send 0
    dataToSend = 0;
  }

  // debug info
#if (RUNMODE == DEBUGINFO)
  UARTprintf("DSP: %d\r\n", dataToSend);
#endif

  // send to SPI
  QueueSend(QueueSPITX, &dataToSend);

  // recieve from SPI
  if(QueueReceive(QueueSPIRX, &FromSPI)){
    // decide if it is for pan or tilt and then put position accordingly in a queue
    if((FromSPI & MASK_MESSAGERECEIVE_MOTOR)){//pan
      FromSPI = FromSPI & MASK_MESSAGE_POSITION;
      QueueOverwrite(QueuePositionPan, &FromSPI);
    } else {//tilt
      FromSPI = FromSPI & MASK_MESSAGE_POSITION;
      QueueOverwrite(QueuePositionTilt, &FromSPI);
    }
  }
}
/****************************** End Of Module *******************************/
