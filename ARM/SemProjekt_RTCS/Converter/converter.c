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
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define RETURNVALUE 2
#define RUNMODE RETURNVALUE



#define cameraAnglePan 70
#define cameraAngleTilt 30
#define cameraPixelPan 1920
#define cameraPixelTilt 1080
#define ticksPerDegree 3
#define convertPixelToPan (ticksPerDegree * cameraAnglePan)/cameraPixelPan
#define convertPixelToTilt (ticksPerDegree * cameraAngleTilt)/cameraPixelTilt
#define pictureOrigoPan 960
#define pictureOrigoTilt 540

#define ticksPerRotation 1080
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
  INT8U data = 0;
  INT16U currentPosition = 0;
  static INT32S decValue = 0;

  // get input from UART
  if (QueueReceive(QueueUARTRX, &data)) {
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
          // send position to tilt
          QueueOverwrite(QueueGoToPositionTilt, &decValue);
          // debug
#if RUNMODE == DEBUGINFO
    UARTprintf("FT: %d\r\n",decValue);
#endif
        }
        else if (motor == PAN) {
          // send position to pan
          QueueOverwrite(QueueGoToPositionPan, &decValue);
          // debuginfo
#if RUNMODE == DEBUGINFO
    UARTprintf("FP: %d\r\n",decValue);
#endif
        }
      }
      else if (function == PIXEL) {
        if (motor == TILT) {
          // get current position
          currentPosition = QueuePeek(QueuePositionTilt, &currentPosition);
          // find persons offset from center (center of picture)
          decValue -= pictureOrigoTilt;
          // convert to ticks
          decValue *= convertPixelToTilt;
          // add to current position
          decValue += currentPosition;
          // adjust for out of scope
          decValue %= ticksPerRotation;
          // send
          QueueOverwrite(QueueGoToPositionTilt, &decValue);
          // debuginfo
#if RUNMODE == DEBUGINFO
    UARTprintf("PT: %d\r\n",decValue);
#endif
        }
        else if (motor == PAN) {
          // get current position
          currentPosition = QueuePeek(QueuePositionPan, &currentPosition);
          // find persons offset form center
          decValue -= pictureOrigoPan;
          // convert to ticks
          decValue *= convertPixelToPan;
          // add to current position
          decValue += currentPosition;
          // adjust for out of scope
          decValue %= ticksPerRotation;
          // send
          QueueOverwrite(QueueGoToPositionPan, &decValue);
          // debuginfo
#if RUNMODE == DEBUGINFO
    UARTprintf("PP: %d\r\n",decValue);
#endif
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
}
/****************************** End Of Module *******************************/
