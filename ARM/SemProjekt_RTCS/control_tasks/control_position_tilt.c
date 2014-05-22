/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: control_position_tilt.c
 *
 * PROJECT....: G3 - Tracking system utilizing a pan/tilt systems
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140501  G3   Module created.
 * 1405XX  G3   Module modified
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_position_tilt.h"
#include "RTCS/rtcs.h"
#include "debug/debug.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTPOSITION 2
#define INTENSEDEBUG 3
#define DEBUGTIME 4

#define RUN_MODE NORMAL //
#define DEFAULTPOSITION_TILT 0

#define PID_RUN_INTERVAL 30 // ticks

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
void init_tilt_position_task(){
  //UARTprintf("Starting Position Tilt Task\r\n");
  _start(TILT_POSITION_TASK, MILLI_SEC(0));
}

void tilt_position_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  PID_RUN_INTERVAL
 *****************************************************************************/
{
#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. Pos. tilt: Start.\r\n");
#endif
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
#if RUN_MODE == DEBUGTIME
    debug_pin(ON);
#endif
    pid_interval_counter = PID_RUN_INTERVAL;

#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. Pos. tilt: Loop entered.\r\n");
#endif
    // get current and goto position
    QueuePeek(&QueuePositionTilt, &current_position);
    if (!QueuePeek(&QueueGoToPositionTilt, &goToPosition)) {
      goToPosition = DEFAULTPOSITION_TILT;
    }

#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. Pos. tilt: Peeks made.\r\n");
#endif
    // calc error
    error = goToPosition - current_position;

    //shortest path correction
    if(error > TICKS_PER_FRAME_ROTATION/2){
    	error -= TICKS_PER_FRAME_ROTATION;
    }
    else if (error < -TICKS_PER_FRAME_ROTATION/2) {
      error += TICKS_PER_FRAME_ROTATION;
    }

    // calculate differential and integral error
    Derror = (error - last_error)*IDT;
    Ierror+=error;

    // limit integral
    if(Ierror > INTEGRATORLIMIT*IDT){
    	Ierror = INTEGRATORLIMIT*IDT;
    }
    // lukas limit 200*IDT
    else if(Ierror < -INTEGRATORLIMIT*IDT){
    	Ierror = -INTEGRATORLIMIT*IDT;
    }

    // calc wanted speed
    set_speed = error*Kp + (Ierror*Ki)/IDT + Derror*Kd;

    // limit wanted speed
    if(set_speed > MAXSPEED_LIMIT){
    	set_speed = MAXSPEED_LIMIT;
    }
    else if(set_speed < -MAXSPEED_LIMIT){
    	set_speed = -MAXSPEED_LIMIT;
    }

    // send wanted speed
    QueueOverwrite(&QueueTiltSpeed, &set_speed);

#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. Pos. tilt: Speed overwritten.\r\n");
#endif
    // plot position
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
#if RUN_MODE == DEBUGTIME
    debug_pin(OFF);
#endif
  }
#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. Pos. tilt: Exit.\r\n");
#endif
}
/****************************** End Of Module *******************************/
