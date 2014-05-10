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
#include "control_tasks/control_speed_tilt.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTSPEED 2

#define RUN_MODE PLOTSPEED //

#define PID_RUN_INTERVAL 50 // ticks
#define PID_SPEED_CALC_INTERVAL 2  // length of time over which the speed is averaged as a multiple of PID_RUN_INTERVAL

#define Kp 0.13
#define Ki 0.0015
#define Kd 0.045



/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_pan_speed_task(){
  _start(TILT_SPEED_TASK, MILLI_SEC(0));
}

void pan_speed_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  > 5 times system frequency
 *****************************************************************************/
{
  static INT16U current_position = 0;

  // direction calc vars
  // static INT16U last_position = 0;
  // static INT8U turn_direction;
  //static INT16U last_position = 0;

  // PID controller vars
  static INT16U last_error = 0;
  INT16S error, Derror, Ierror = 0, set_pwm;
  static INT16U position[PID_SPEED_CALC_INTERVAL +1] = {0};

  static INT8U i = 1;

  INT16S set_speed_tilt = 500;

  INT8U var;

  static INT16U pid_interval_counter = PID_RUN_INTERVAL;
  static INT16S last_pwm = 0;
  INT8U direction = 0;
  INT32S current_speed = 0;

  INT16U dt = 1000/(PID_RUN_INTERVAL*T_TICK*PID_SPEED_CALC_INTERVAL);


  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    //get speed
   // QueueReceive(QueueTiltSpeed, &set_speed_tilt);

#if (RUN_MODE == DEBUGINFO)
    UARTprintf("ss: %d\r\n", set_speed_tilt);
#endif


    // get position and empty QueuePositionTILT-queue
    QueueReceive(QueuePositionTILT, &current_position);

    // save positions
    for (var = 0; var < (PID_SPEED_CALC_INTERVAL); ++var) {
      position[var] = position[var+1];
    }
    position[(PID_SPEED_CALC_INTERVAL)] = current_position; // 0 = oldest, PID_... = Newest

    // calc current speed
    current_speed = (position[PID_SPEED_CALC_INTERVAL] - position[0])*dt;

    // adjust current speed for overflow due to direction reset (1079 -> 0 and 0 -> 1079)
    if(current_speed < -1500){ // CW
      current_speed += 1080*dt;
    }
    else if (current_speed > 1500) { // CCW
      current_speed -=  1080*dt;
    }

    // error calc
    error = set_speed_tilt - current_speed;
    Derror = (error - last_error)*dt;
    Ierror += error;

    if(Ierror > 100*dt){
    	Ierror = 100*dt;
    }
    else if(Ierror < -100*dt){
    	Ierror = -100*dt;
    }
    // calculate PID
    set_pwm = error*Kp + Derror*Kd + (Ierror*Ki)/dt;
    set_pwm += last_pwm;

    // limit pwm
    if (set_pwm > 255) {
      set_pwm = 255;
    }
    else if (set_pwm < -255) {
      set_pwm = -255;
    }

    // save last pwm and error for next calc
    last_pwm = set_pwm;
    last_error = error;

    // debuginfo
#if (RUN_MODE == DEBUGINFO)
    UARTprintf("cs: %d, pwmO: %d\r\n", current_speed, set_pwm);
#endif

    // direction
    if(set_pwm > 0 ){
      direction = 0b10;
    } else {
      direction = 0b01;
      set_pwm = -set_pwm;
    }

    set_pwm = (direction << 8) | (set_pwm & 0xFF);
    QueueSend(QueuePWMOutTilt, &set_pwm);

#if (RUN_MODE == PLOTSPEED)
    if(--i == 0){
      i = 4;
      UARTCharPut(0,'|');
      UARTCharPut(0, current_speed>> 8);
      UARTCharPut(0, current_speed);
    }
#endif

  }
}
/****************************** End Of Module *******************************/
