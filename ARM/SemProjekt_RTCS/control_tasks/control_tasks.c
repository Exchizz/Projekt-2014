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
 * 0902012  KHA   Module created.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks.h"
/*****************************    Defines    ********************************/
#define PID_RUN_INTERVAL 20 // ticks
#define PID_SPEED_CALC_INTERVAL 2  // length of time over which the speed is averaged as a multiple of PID_RUN_INTERVAL

#define Kp 0.16
#define Kd 0.005
#define Ki 0


/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_tilt_control_task(){
	_start(TILT_CONTROL_TASK, MILLI_SEC(0));
}

void tilt_control_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 : 1000 ms
 *****************************************************************************/
{
	INT16U current_position = 0;

	// direction calc vars
	static INT16U last_position = 0;


	// PID controller vars
	static INT16U last_error = 0;
	static INT16U position[PID_SPEED_CALC_INTERVAL +1] = {0};

	INT16S set_speed_tilt = 500;

	INT8U var;

	static INT16U pid_interval_counter = PID_RUN_INTERVAL;
	static INT16S last_pwm = 0;
	INT16S error, Derror, Ierror = 0, set_pwm;
	INT8U direction = 0;
	INT32S current_speed = 0;
	static INT8U turn_direction;

	INT16U dt = 1000/(PID_RUN_INTERVAL*T_TICK*PID_SPEED_CALC_INTERVAL);


	// calculate direction of rotation
	//CW
	if (QueueReceive(QueuePositionTILT, &current_position)) {
		if((last_position > 900 && current_position < 100) || ((current_position > last_position) && !(current_position > 900 && last_position < 100))){
			turn_direction = TRUE;
		} else if ((current_position > 900 && last_position < 100) || ((current_position < last_position) && !(last_position > 900 && current_position < 100))) { // CCW
			turn_direction = FALSE;
		}
		//UARTprintf("lp: %d, cp: %d \r\n",last_position,current_position);
		last_position = current_position;
	}



	if(!(--pid_interval_counter)){
		pid_interval_counter = PID_RUN_INTERVAL;

		// save position
		for (var = 0; var < (PID_SPEED_CALC_INTERVAL); ++var) {
			position[var] = position[var+1];
		}
		position[(PID_SPEED_CALC_INTERVAL)] = current_position; // 0 = oldest, PID_... = Newest

		// calc current speed
		current_speed = (position[PID_SPEED_CALC_INTERVAL] - position[0])*dt;

		//UARTprintf("speed: %d \r\n",current_speed);

		// adjust current speed for overflow
		if(turn_direction && current_speed < 0){ // CW
			current_speed += 1080*dt;
			//UARTprintf("add \r\n");
		}
		else if ((!(turn_direction)) && (current_speed > 0)) { // CCW
			current_speed -=  1080*dt;
			//UARTprintf("subb \r\n");
		}

		UARTprintf("speed: %d \r\n",current_speed);

		// error calc
		error = set_speed_tilt - current_speed;
		//QueueSend(QueueUARTTX, &error);
		//UARTprintf("%d\r\n", error);
		Derror = (error-last_error)*dt;
		Ierror+=error*dt;


		set_pwm = error*Kp + Derror*Kd + Ierror*Ki;
		set_pwm += last_pwm;

		// limit pwm
		if (set_pwm > 255) {
			set_pwm = 255;
		}
		if (set_pwm < -255) {
			set_pwm = -255;
		}

		last_pwm = (set_pwm);

		//UARTprintf("setpwm: %d\r\n", (set_pwm));

		// direction
		if(set_pwm > 0 ){
			direction = 0b10;
		} else {
			direction = 0b01;
			set_pwm = -set_pwm;
		}

		//UARTprintf("speed: %d \r\n", current_speed);
		//UARTprintf("current_position: %d, last_position: %d, PWM: %d, current_speed: %d, error: %d, time: %d, direction: %d \r\n", current_position, last_position, out, current_speed, (current_position-last_position), dt, direction);
		//UARTprintf("cs: %d, er: %d, os: %d, ls: %d\r\n", current_speed, error, set_pwm, last_pwm);
		//UARTprintf("cs: %d, er: %d, pwm: %d, cp: %d, lp: %d\r\n", current_speed, error, last_pwm, position[PID_SPEED_CALC_INTERVAL], position[0]);




		set_pwm = (direction << 8) | (set_pwm & 0xFF);
		QueueSend(QueuePWMOutTilt, &set_pwm);

		last_error = error;
	}
}
/****************************** End Of Module *******************************/
