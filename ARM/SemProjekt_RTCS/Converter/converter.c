/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.:
 *
 * PROJECT....:
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140511  G3   Module created.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "Converter/converter.h"
#include "communication/communication_task.h"
/*****************************    Defines    ********************************/
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
enum CONVERTER_STATES {WAIT_FUNC, RECIEVE_MOTOR, RECIEVE_DEC_NUM};
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
    switch (data) {
    case 'p':
    function = PIXEL;
    ReceiveDecNumState = THOUSANDS;
    receive_state = RECIEVE_MOTOR;
    UARTprintf("px\r\n");
    break;
    case 'f':
    function = FIXED;
    ReceiveDecNumState = THOUSANDS;
    receive_state = RECIEVE_MOTOR;
    UARTprintf("fi\r\n");
    break;
    default:
    break;
    }
    break;

    case RECIEVE_MOTOR:
    switch (data) {
    case 'p':
    motor = PAN;
    decValue = 0;
    receive_state = RECIEVE_DEC_NUM;
    UARTprintf("pan\r\n");
    break;
    case 't':
    motor = TILT;
    decValue = 0;
    receive_state = RECIEVE_DEC_NUM;
    UARTprintf("tilt\r\n");
    break;
    default:
    break;
    }
    break;

    case RECIEVE_DEC_NUM:
    switch (ReceiveDecNumState) {
    case TENTHOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = THOUSANDS;
      UARTprintf("TT\r\n");
    }
    break;
    case THOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = HUNDREDS;
      UARTprintf("t\r\n");
    }
    break;
    case HUNDREDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = TENS;
      UARTprintf("h\r\n");
    }
    break;
    case TENS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      ReceiveDecNumState = ONES;
      UARTprintf("t\r\n");
    }
    break;
    case ONES:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');

      UARTprintf("o\r\n");
      // process data if fixed or pan else other function might requirer different jump.
      if (function == FIXED) {
        decValue %= 1080;
        if (motor == TILT) {
          QueueOverwrite(QueueGoToPositionTilt, &decValue);
          UARTprintf("fiT: %d\r\n",decValue);
        }
        else if (motor == PAN) {
          QueueOverwrite(QueueGoToPositionPan, &decValue);
          UARTprintf("fiP\r\n");
        }
      }
      else if (function == PIXEL) {
        if (motor == TILT) {
          currentPosition = QueuePeek(QueuePositionTilt, &currentPosition);
          // convert to ticks
          decValue -= pictureOrigoTilt;
          decValue *= convertPixelToTilt;
          // add to current position
          decValue += currentPosition;
          // adjust for out of scope
          decValue %= ticksPerRotation;
          // send
          QueueOverwrite(QueueGoToPositionTilt, &decValue);
          UARTprintf("pxT\r\n");
        }
        else if (motor == PAN) {
          currentPosition = QueuePeek(QueuePositionPan, &currentPosition);
          // convert to ticks
          decValue -= pictureOrigoPan;
          decValue *= convertPixelToPan;
          // add to current position
          decValue += currentPosition;
          // adjust for out of scope
          decValue %= ticksPerRotation;
          // send
          QueueOverwrite(QueueGoToPositionPan, &decValue);
          UARTprintf("pxP\r\n");
        }
      }
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
