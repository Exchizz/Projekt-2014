/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: control_speed_pan.c
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
 * 140501  G3   Module created.
 * 1405XX  G3   Module modified.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_speed_pan.h"
#include "RTCS/rtcscnf.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTSPEED 2

#define RUN_MODE NORMAL //

#define PID_RUN_INTERVAL 50 // ticks
#define PID_SPEED_CALC_INTERVAL 2  // length of time over which the speed is averaged as a multiple of PID_RUN_INTERVAL

#define Kp 0.13
#define Ki 0.0015
#define Kd 0.045

#define IDT (1000/(PID_RUN_INTERVAL*T_TICK*PID_SPEED_CALC_INTERVAL))

#define TICKS_PER_FRAME_ROTATION 1080
#define INTEGRATORLIMIT 100
#define MAXSPEED_LIMIT 1500 // ticks/s
#define MAXPWMVALUE 255
#define MAXPWM_MASK 0xFF
#define MOTOR_DIRECTION_FORWARDS 0b01
#define MOTOR_DIRECTION_BACKWARDS 0b10

/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_pan_speed_task(){
  _start(PAN_SPEED_TASK, MILLI_SEC(0));
}

void pan_speed_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  PID_RUN_INTERVAL
 *****************************************************************************/
{
  INT16U current_position = 0;

  // PID controller vars
  static INT16U last_error = 0;
  INT16S error, Derror, set_pwm;
  static INT16S Ierror = 0;
  static INT16U position[PID_SPEED_CALC_INTERVAL +1] = {0};

  static INT8U i = 1;

  INT16S set_speed_pan = 0;

  INT8U var;

  static INT16U pid_interval_counter = PID_RUN_INTERVAL;
  static INT16S last_pwm = 0;
  INT8U direction = 0;
  INT32S current_speed = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    //get wanted speed
    QueuePeek(&QueuePanSpeed, &set_speed_pan);

    // get current position
    QueuePeek(&QueuePositionPan, &current_position);

    // save current position in array
    for (var = 0; var < (PID_SPEED_CALC_INTERVAL); ++var) {
      position[var] = position[var+1];
    }
    position[(PID_SPEED_CALC_INTERVAL)] = current_position; // 0 = oldest, PID_... = Newest

    // calc current speed
    current_speed = (position[PID_SPEED_CALC_INTERVAL] - position[0])*IDT;

    // adjust current speed for overflow due to direction reset (1079 -> 0 and 0 -> 1079)
    if(current_speed < -MAXSPEED_LIMIT){ // CW
      current_speed += TICKS_PER_FRAME_ROTATION*IDT;
    }
    else if (current_speed > MAXSPEED_LIMIT) { // CCW
      current_speed -=  TICKS_PER_FRAME_ROTATION*IDT;
    }

    // error calc
    error = set_speed_pan - current_speed;
    Derror = (error - last_error)*IDT;
    Ierror += error;

    // integral limit
    if(Ierror > INTEGRATORLIMIT*IDT){
    	Ierror = INTEGRATORLIMIT*IDT;
    }
    else if(Ierror < -INTEGRATORLIMIT*IDT){
    	Ierror = -INTEGRATORLIMIT*IDT;
    }

    // calculate set pwm
    set_pwm = error*Kp + Derror*Kd + (Ierror*Ki)/IDT;
    set_pwm += last_pwm;

    // limit pwm
    if (set_pwm > MAXPWMVALUE) {
      set_pwm = MAXPWMVALUE;
    }
    else if (set_pwm < -MAXPWMVALUE) {
      set_pwm = -MAXPWMVALUE;
    }

    // save last pwm and error for next calc
    last_pwm = set_pwm;
    last_error = error;

    // debuginfo
#if (RUN_MODE == DEBUGINFO)
    UARTprintf("GSP: %d, CSP: %d, PWMP: %d\r\n", set_speed_pan, current_speed, set_pwm);
#endif

    // determine motor direction
    if(set_pwm > 0 ){
      direction = MOTOR_DIRECTION_FORWARDS;
    } else {
      direction = MOTOR_DIRECTION_BACKWARDS;
      set_pwm = -set_pwm;
    }

    // ready message to send to pan and send
    set_pwm = (direction << 8) | (set_pwm & MAXPWM_MASK);
    QueueSend(&QueuePWMOutPan, &set_pwm);

    // plotspeed
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
