/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: control_position_pan.c
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
 * 140501  G3   Module created. (copy of tilt)
 * 1405XX  G3   Module modified for pan. (stopband etc.)
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_position_pan.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTPOSITION 2

#define RUN_MODE NORMAL //
#define DEFAULTPOSITIONPAN 0

// both adjusted  to not bump into stopper. value can vary due to index-magnet ranging over multiple ticks.
#define STOP_BAND_START 205 // when the band were it can't be starts (~221-227)
#define STOP_BAND_STOP 860 // ^... stops (~840-846)

#define PID_RUN_INTERVAL 20 // ticks

#define Kp 1
#define Ki 0.05
#define Kd 0.2

#define IDT (1000/(PID_RUN_INTERVAL*T_TICK))

#define TICKS_PER_FRAME_ROTATION 1080
#define INTEGRATORLIMIT 50
#define MAXSPEED_LIMIT 1500 // ticks/s
/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_pan_position_task(){
  _start(PAN_POSITION_TASK, MILLI_SEC(0));
}

void pan_position_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  PID_RUN_INTERVAL
 *****************************************************************************/
{
  INT16U current_position = 0;

  // PID controller vars
  static INT16S last_error = 0;
  INT16S error, Derror;
  static INT32S Ierror = 0;
  static INT16U pid_interval_counter = PID_RUN_INTERVAL;

  INT16U goToPosition = 0;

  INT16S set_speed = 0;
  static INT8U i = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    // get goto and current position
    QueuePeek(QueuePositionPan, &current_position);
    if (!QueuePeek(QueueGoToPositionPan, &goToPosition)) {
      goToPosition = DEFAULTPOSITIONPAN;
    }

    // test for valid position (stop band)
    // if position in stopband go to nearest valid position (stopband-stop/start position)
    if (goToPosition > STOP_BAND_START && goToPosition < STOP_BAND_STOP) {
      if (goToPosition < (STOP_BAND_START + STOP_BAND_STOP)/2) {
        goToPosition = STOP_BAND_START;
      }
      else {
        goToPosition = STOP_BAND_STOP;
      }
    }

    // calc error
    error = goToPosition - current_position;

    //shortest path correction.
    if(error > TICKS_PER_FRAME_ROTATION/2){
    	error -= TICKS_PER_FRAME_ROTATION;
    }
    else if (error < -TICKS_PER_FRAME_ROTATION/2) {
      error += TICKS_PER_FRAME_ROTATION;
    }

    // if prevent go through region is needed implement here
    // will no be needed because pan freedom < 180* and shortest
    // path will then always be through passable area

    // calc differential and integral value
    Derror = (error - last_error)*IDT;
    Ierror+=error;

    // limit error
    if(Ierror > INTEGRATORLIMIT*IDT){
    	Ierror = INTEGRATORLIMIT*IDT;
    }
    // lukas limit 200*IDT
    else if(Ierror < -INTEGRATORLIMIT*IDT){
    	Ierror = -INTEGRATORLIMIT*IDT;
    }

    // calculate the wanted speed (ticks/s)
    set_speed = error*Kp + (Ierror*Ki)/IDT + Derror*Kd;

    // limit set_speed. motor not able to be faster than 1200, but higher will force to constant 255 PWM.
    if(set_speed > MAXSPEED_LIMIT){
    	set_speed = MAXSPEED_LIMIT;
    }
    else if(set_speed < -MAXSPEED_LIMIT){
    	set_speed = - MAXSPEED_LIMIT;
    }
    // send the speed
    QueueOverwrite(QueuePanSpeed, &set_speed);

    // debuginfo
#if RUN_MODE == DEBUGINFO
    UARTprintf("GPP: %d, WSP: %d\r\n",goToPosition, set_speed);
#endif

    // plot current position
#if (RUN_MODE == PLOTPOSITION)
  if(--i == 0){
    i = 5;
    UARTCharPut(0,'|');
    UARTCharPut(0, current_position >> 8);
    UARTCharPut(0, current_position);
  }
#endif

  // save last error
  last_error = error;
  }
}
/****************************** End Of Module *******************************/
