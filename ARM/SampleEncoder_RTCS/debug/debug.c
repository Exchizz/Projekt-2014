#include "debug/debug.h"

void debug_pin(INT8U state){
	static INT8U current_state = OFF;
	switch(state){
		case ON:
			GPIO_PORTG_DATA_R |=  PG1;
		break;

		case OFF:
			GPIO_PORTG_DATA_R &= ~(PG1);
		break;
		case TOGGLE:
			if(current_state == ON) {
				current_state = OFF;
				GPIO_PORTG_DATA_R &= ~(PG1);
			} else {
				current_state = ON;
				GPIO_PORTG_DATA_R |=  PG1;
			}
		break;
	}
}


void debug_pin_red(INT8U state){
	static INT8U current_state_red = OFF;
	switch(state){
		case ON:
			GPIO_PORTD_DATA_R |=  PD6;
		break;

		case OFF:
			GPIO_PORTD_DATA_R &= ~(PD6);
		break;
		case TOGGLE:
			if(current_state_red == ON) {
				current_state_red = OFF;
				GPIO_PORTD_DATA_R &= ~(PD6);
			} else {
				current_state_red = ON;
				GPIO_PORTD_DATA_R |=  PD6;
			}
		break;
	}
}
