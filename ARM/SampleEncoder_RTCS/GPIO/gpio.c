#include "gpio/gpio.h"

void gpio_init(){

	// Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;

	// Set the direction.
	GPIO_PORTG_DIR_R |= PG1;
	GPIO_PORTF_DIR_R |= PF0;
	GPIO_PORTD_DIR_R |= PD6;

	// Enable the GPIO pins for digital function
	GPIO_PORTG_DEN_R |= PG1;
	GPIO_PORTF_DEN_R |= PF0;
	GPIO_PORTD_DEN_R |= PD6;

	//Set pins high/lov
	GPIO_PORTF_DATA_R |= PF0;
	GPIO_PORTD_DATA_R |= PD6;
}
