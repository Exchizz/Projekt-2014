/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: debug.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "debug.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

//*****************************************************************************
//
// The error routine that is called by the assert macro.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
	DEBUG_CRITICAL("ASSERT ERROR: File:%s  Line:%d\n",pcFilename, ulLine);
	while(1);
}
#endif

void debug_pin(INT8U state){
	if(state == OFF)
		SET_BIT(GPIO_PORTG_DATA_R, PG1);
	else
		CLEAR_BIT(GPIO_PORTG_DATA_R, PG1);
}

void debug_pin_init(){
	INT8S dummy;

	// Enable the GPIO port that is used for the on-board LED.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;

	// Do a dummy read to insert a few cycles after enabling the peripheral.
	dummy = SYSCTL_RCGC2_R;

	// Set the direction as output (PG0).
	GPIO_PORTG_DIR_R |= 0x02;

	// Enable the GPIO pins for digital function (PG0).
	GPIO_PORTG_DEN_R |= 0x02;
}

/****************************** End Of Module *******************************/
