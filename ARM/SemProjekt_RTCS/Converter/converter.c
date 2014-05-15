/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: converter.c
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
 * 140511  G3   Module created.
 * 1405XX  G3   Module modified.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "Converter/converter.h"
#include "communication/communication_task.h"
#include "RTCS/rtcs.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define RETURNVALUE 2
#define INTENSEDEBUG 3
#define RETURNSETPOSITION 4

#define RUNMODE RETURNSETPOSITION

#define cameraAnglePan 70
#define cameraAngleTilt 30
#define cameraPixelPan 1920
#define cameraPixelTilt 1080
#define ticksPerDegree 3
#define pictureOrigoPan 960
#define pictureOrigoTilt 540

#define ticksPerRotation 1080

#define MINPOSITIONCHANGE 9

/*****************************   Constants   ********************************/
enum CONVERTER_STATES {WAIT_FUNC, RECEIVE_MOTOR, RECEIVE_DEC_NUM};
enum MOTOR_STATES {PAN, TILT};
enum FUNCTION_STATES {PIXEL, FIXED};
enum DEC_NUM_STATES {TENTHOUSANDS, THOUSANDS, HUNDREDS, TENS, ONES};

/*****************************   Variables   ********************************/
enum CONVERTER_STATES receive_state = WAIT_FUNC;
enum MOTOR_STATES motor = PAN;
enum FUNCTION_STATES function = PIXEL;
enum DEC_NUM_STATES ReceiveDecNumState = THOUSANDS;

/*****************************   Functions   ********************************/
void init_converter_task(){
  UARTprintf("Starting Converter Task\r\n");
  _start(CONVERTER_TASK, MILLI_SEC(0));
}

void converter_task()
/*****************************************************************************
 *   Input    :         -
 *   Output   :         -
 *   Function :
 *   Run @       :  > 5 times system frequency
 *****************************************************************************/
{
#if RUNMODE == INTENSEDEBUG
  UARTprintf("Converter: Start.\r\n");
#endif
  INT8U data = 0;
  INT16U currentPosition = 0;
  static INT32S decValue = 0;

#if RUNMODE == INTENSEDEBUG
  UARTprintf("Converter: Just before if(Rec).\r\n");
#endif
  // get input from UART
  if (QueueReceive(&QueueUARTRX, &data)) {
#if RUNMODE == INTENSEDEBUG
  UARTprintf("Converter: Data received.\r\n");
#endif
    switch (receive_state) {
    case WAIT_FUNC:
    // switch if pixel or fixed
    switch (data) {
    case 'p':
    function = PIXEL;
    ReceiveDecNumState = THOUSANDS;
    receive_state = RECEIVE_MOTOR;
    // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("px\r\n");
#endif
    break;
    case 'f':
    function = FIXED;
    ReceiveDecNumState = THOUSANDS;
    receive_state = RECEIVE_MOTOR;
    // debug
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("fi\r\n");
#endif
    break;
    default:
    break;
    }
    break;

    case RECEIVE_MOTOR:
    // switch depending on pan or tilt
    switch (data) {
    case 'p':
    motor = PAN;
    decValue = 0;
    receive_state = RECEIVE_DEC_NUM;
    // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("pan\r\n");
#endif
    break;
    case 't':
    motor = TILT;
    decValue = 0;
    receive_state = RECEIVE_DEC_NUM;
    // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("tilt\r\n");
#endif
    break;
    default:
    break;
    }
    break;

    case RECEIVE_DEC_NUM:
    // receive a number and execute command in ones (at last dec recieve).
    switch (ReceiveDecNumState) {
    case TENTHOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = THOUSANDS;
      // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case THOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = HUNDREDS;
      // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case HUNDREDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = TENS;
      // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case TENS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = ONES;
      // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case ONES:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c\r\n",data);
#endif

      // process data if fixed or pan else other function might requirer different jump.
      if (function == FIXED) {
        // prevent invalid function
        decValue %= ticksPerRotation;
        if (motor == TILT) {
          QueuePeek(&QueueGoToPositionTilt, &currentPosition);
          // find difference in position
          if (currentPosition > decValue) {
            currentPosition -= decValue;
          }
          else {
            currentPosition = decValue - currentPosition;
          }
          // if difference is more than min change
          if (currentPosition <= -MINPOSITIONCHANGE || currentPosition >= MINPOSITIONCHANGE) {
            // send position to tilt
            QueueOverwrite(&QueueGoToPositionTilt, &decValue);
            // debug
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNSETPOSITION
            UARTprintf("FT: %d\r\n",decValue);
#endif
          }
        }
        else if (motor == PAN) {
          // get current go to position
          QueuePeek(&QueueGoToPositionTilt, &currentPosition);
          // find difference in position
          if (currentPosition > decValue) {
            currentPosition -= decValue;
          }
          else {
            currentPosition = decValue - currentPosition;
          }
          // if difference is more than min change
          if (currentPosition <= -MINPOSITIONCHANGE || currentPosition >= MINPOSITIONCHANGE) {
            // send position to pan
            QueueOverwrite(&QueueGoToPositionPan, &decValue);
            // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNSETPOSITION
            UARTprintf("FP: %d\r\n",decValue);
#endif
          }
        }
      }
      else if (function == PIXEL) {
        if (motor == TILT) {
          // get current position
          QueuePeek(&QueuePositionTilt, &currentPosition);
          // adjust for out of picture
          if (decValue > cameraPixelTilt) {
            decValue = cameraPixelTilt;
          }
          // find persons offset from center (center of picture)
          decValue -= pictureOrigoTilt;
          // convert to ticks
          decValue *= ticksPerDegree;
          decValue *= cameraAngleTilt;
          decValue /= cameraPixelTilt;
          // add to current position
          decValue += currentPosition;
          // adjust goto position for out of scope
          if (decValue < 0) {
            decValue += ticksPerRotation;
          }
          decValue %= ticksPerRotation;
          // get current go to possition
          QueuePeek(&QueueGoToPositionTilt, &currentPosition);
          // find difference in position
          if (currentPosition > decValue) {
            currentPosition -= decValue;
          }
          else {
            currentPosition = decValue - currentPosition;
          }
          // if difference is more than min change
          if (currentPosition <= -MINPOSITIONCHANGE || currentPosition >= MINPOSITIONCHANGE) {
            // send
            QueueOverwrite(&QueueGoToPositionTilt, &decValue);
            // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNSETPOSITION
            UARTprintf("PT: %d\r\n",decValue);
#endif
          }
        }
        else if (motor == PAN) {
          // get current position
          QueuePeek(&QueuePositionPan, &currentPosition);
          // adjust for out of picture
          if (decValue > cameraPixelPan) {
            decValue = cameraPixelPan;
          }
          // find persons offset form center
          decValue -= pictureOrigoPan;
          // convert to ticks
          decValue *= ticksPerDegree;
          decValue *= cameraAnglePan;
          decValue /= cameraPixelPan;
          // add to current position
          decValue += currentPosition;
          // adjust for out of scope
          if (decValue < 0) {
            decValue += ticksPerRotation;
          }
          decValue %= ticksPerRotation;
          // get current go to position
          QueuePeek(&QueueGoToPositionPan, &currentPosition);
          // find difference in position
          if (currentPosition > decValue) {
            currentPosition -= decValue;
          }
          else {
            currentPosition = decValue - currentPosition;
          }
          // if difference is more than min change
          if (currentPosition <= -MINPOSITIONCHANGE || currentPosition >= MINPOSITIONCHANGE) {
            // send
            QueueOverwrite(&QueueGoToPositionPan, &decValue);
            // debuginfo
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNSETPOSITION
            UARTprintf("PP: %d\r\n",decValue);
#endif
          }
        }
      }
      // change state
      receive_state = WAIT_FUNC;
    }
    break;
    default:
      // do nothing if not ones, tens, ...
    break;
    }
    break;

    default:
      // do nothing if receive state not known
    break;
    }
  }
#if RUNMODE == INTENSEDEBUG
  UARTprintf("Converter: Exit.\r\n");
#endif
}
/****************************** End Of Module *******************************/
