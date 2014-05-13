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
enum CONVERTER_STATES {WAIT_FUNC, RECIEVE_MOTOR, RECIEVE_DEC_NUM};
enum MOTOR_STATES {PAN, TILT};
enum FUNCTION_STATES {PIXEL, FIXED};
enum DEC_NUM_STATES {TENTHOUSANDS, THOUSANDS, HUNDREDS, TENS, ONES};

/*****************************   Variables   ********************************/
volatile enum CONVERTER_STATES recieve_state = WAIT_FUNC;
volatile enum MOTOR_STATES motor = PAN;
volatile enum FUNCTION_STATES function = PIXEL;
volatile enum DEC_NUM_STATES RecieveDecNumState = THOUSANDS;

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
    switch (recieve_state) {
    case WAIT_FUNC:
    switch (data) {
    case 'p':
    function = PIXEL;
    RecieveDecNumState = THOUSANDS;
    recieve_state = RECIEVE_MOTOR;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("px\r\n");
#endif
    break;
    case 'f':
    function = FIXED;
    RecieveDecNumState = THOUSANDS;
    recieve_state = RECIEVE_MOTOR;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("fi\r\n");
#endif
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
    recieve_state = RECIEVE_DEC_NUM;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("pan\r\n");
#endif
    break;
    case 't':
    motor = TILT;
    decValue = 0;
    recieve_state = RECIEVE_DEC_NUM;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("tilt\r\n");
#endif
    break;
    default:
    break;
    }
    break;

    case RECIEVE_DEC_NUM:
    switch (RecieveDecNumState) {
    case TENTHOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      RecieveDecNumState = THOUSANDS;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case THOUSANDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      RecieveDecNumState = HUNDREDS;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case HUNDREDS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      RecieveDecNumState = TENS;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case TENS:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
      RecieveDecNumState = ONES;
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c",data);
#endif
    }
    break;
    case ONES:
    if (data <= '9' && data >= '0') {
      decValue *= 10;
      decValue += (data - '0');
#if RUNMODE == DEBUGINFO || RUNMODE == RETURNVALUE
    UARTprintf("%c\r\n",data);
#endif

      // process data if fixed or pan else other function might requirer different jump.
      if (function == FIXED) {
        decValue %= 1080;
        if (motor == TILT) {
          QueueOverwrite(QueueGoToPositionTilt, &decValue);
#if RUNMODE == DEBUGINFO
    UARTprintf("FT: %d\r\n",decValue);
#endif
        }
        else if (motor == PAN) {
          QueueOverwrite(QueueGoToPositionPan, &decValue);
#if RUNMODE == DEBUGINFO
    UARTprintf("FP: %d\r\n",decValue);
#endif
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
#if RUNMODE == DEBUGINFO
    UARTprintf("PT: %d\r\n",decValue);
#endif
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
#if RUNMODE == DEBUGINFO
    UARTprintf("PP: %d\r\n",decValue);
#endif
        }
      }
      recieve_state = WAIT_FUNC;
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



  //QueueSend(QueuePWMOutTilt, &set_pwm);
}
/****************************** End Of Module *******************************/
