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

#define RUN_MODE NORMAL //

#define PID_RUN_INTERVAL 20 // ticks
#define POSITION_SETPOINT	500

#define Kp 0.5 //0.5
#define Ki 3 //1
#define Kd 0
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

  INT16U dt = 1000/(PID_RUN_INTERVAL*T_TICK);
  INT16S set_speed = 0;
  static INT8U i = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    // get position
    QueuePeek(QueuePositionTILT, &current_position);
    error = POSITION_SETPOINT - current_position;

    //shortest path correction(untested)
    if(error > 540){
    	error -= 1080;
    }
    else if (error < -540) {
		error += 1080;
	}
    Derror = (error - last_error)*dt;
    Ierror+=error;

    if(Ierror > 5*dt){
    	Ierror = 5*dt;
    }
    // lukas limit 200*dt
    else if(Ierror < -5*dt){
    	Ierror = -5*dt;
    }

    set_speed = error*Kp + (Ierror*Ki)/dt + Derror*Kd;

    if(set_speed > 1500){
    	set_speed = 1500;
    }
    else if(set_speed < -1500){
    	set_speed = - 1500;
    }
    QueueSend(QueueTiltSpeed, &set_speed);

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
