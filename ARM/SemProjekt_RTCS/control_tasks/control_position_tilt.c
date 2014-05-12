/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: ECP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140501  G3   Module created.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_position_tilt.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTPOSITION 2

#define RUN_MODE DEBUGINFO //
#define  defaultPositionTilt 0

#define PID_RUN_INTERVAL 40 // ticks

#define Kp 1 //0.5
#define Ki 0 //1
#define Kd 0

#define IDT_cpt (1000/(PID_RUN_INTERVAL*T_TICK))

/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_tilt_position_task(){
  _start(TILT_POSITION_TASK, MILLI_SEC(0));
}

void tilt_position_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  > 5 times system frequency
 *****************************************************************************/
{
  INT16U current_position = 0;

  // PID controller vars
  static INT16S last_error = 0;
  INT16S error, Derror;
  static INT32S Ierror = 0;
  static INT16U pid_interval_counter = PID_RUN_INTERVAL;

  INT16U goToPosition = 0;
  static INT16U lastGoToPosition = 0;

  INT16S set_speed = 0;
  static INT8U i = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    // get positions
    QueuePeek(QueuePositionTilt, &current_position);
    if (!QueuePeek(QueueGoToPositionTilt, &goToPosition)) {
      goToPosition = defaultPositionTilt;
    }

    /*
        // if new position
        if (lastGoToPosition != goToPosition) {
          lastGoToPosition = goToPosition;
        }
     */
    error = goToPosition - current_position;

    //shortest path correction
    if(error > 540){
      error -= 1080;
    }
    else if (error < -540) {
      error += 1080;
    }
    Derror = (error - last_error)*IDT_cpt;
    Ierror+=error;

    if(Ierror > 200*IDT_cpt){
    	Ierror = 200*IDT_cpt;
    }
    // lukas limit 200*IDT_cpt
    else if(Ierror < -200*IDT_cpt){
    	Ierror = -200*IDT_cpt;
    }

#if (RUN_MODE == DEBUGINFO)
    UARTprintf("GP:%d,er:%d,ie:%d,de:%d\r\n",goToPosition, error, (Ierror*Ki)/IDT_cpt, Derror);
#endif

    //set_speed = (error*Kp + (Ierror*Ki)/IDT_cpt + Derror*Kd);

    set_speed = error*Kp;

    if(set_speed > 1500){
    	set_speed = 1500;
    }
    else if(set_speed < -1500){
    	set_speed = - 1500;
    }
    QueueOverwrite(QueueTiltSpeed, &set_speed);

#if (RUN_MODE == DEBUGINFO)
    UARTprintf("WS:%d\r\n",set_speed);
#endif


#if (RUN_MODE == PLOTPOSITION)
  if(--i == 0){
    i = 5;
    UARTCharPut(0,'|');
    UARTCharPut(0, current_position >> 8);
    UARTCharPut(0, current_position);
  }
#endif

  last_error = error;
  }
}
/****************************** End Of Module *******************************/
