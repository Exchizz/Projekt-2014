/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			EMP
 * Author(s):		K.Andersen
 * Module:			systick.c
 * Description:	See module specification file (.h-file).
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 090831			KA			Module Created
 * 140311			NL			C Coding Standards for EMP invoked
 *
 */


/***************************** Include files *******************************/
#include "lm3s6965.h"
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define FCPU					5000000

#define SYSTICK_RELOAD_VALUE 250000 // 5 mS

// Missing definitions in lm3s6965.h file
#define NVIC_INT_CTRL_PEND_SYST  	0x04000000  // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 	0x02000000  // Unpend a systick int

#define SYSTICK_PRIORITY		0x7E



#define PF1		1

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
volatile INT16S ticks = 0;


/*****************************   Functions   *******************************/
void init_clk_system()
/*
 * Input		: -
 * Output	: -
 * Function	: Initiates the clock
 */
{
	INT32U dummy;
	// Init clock settings according to datasheet
	// Set Clock speed to 50 Mhz.

	// Step 1
	// Set BYPASS bit
	SYSCTL_RCC_R |= SYSCTL_RCC_BYPASS;
	// Clear USESYS bits
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_USESYSDIV);
	// Enable Main osc.
	SYSCTL_RCC_R |= SYSCTL_RCC_MOSCDIS;

	// Step 2
	// Clear PLL lock flag
	SYSCTL_RIS_R = ~(SYSCTL_RIS_PLLLRIS);
	// Set XTAL value, first clear bits then set
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_XTAL_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_8MHZ;
	// Set OSCSRC, first clear bits then set
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_OSCSRC_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_OSCSRC_MAIN;
	// Clear PWRDN
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_PWRDN);

	// delay
	for (dummy = 200000; dummy >0; dummy--);

	// Step 3
	// Set SYSDIV, first clear bits then set
	// Set to 50 Mhz.
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_SYSDIV_M);
	SYSCTL_RCC_R |= SYSCTL_RCC_SYSDIV_4;
	// Set USESYS bit
	SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV;

	// Step 4
	// wait for PLL lock
	while(!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));

	// Step 5
	// Clear BYPASS bit
	SYSCTL_RCC_R &= ~(SYSCTL_RCC_BYPASS);

	// delay
	for (dummy = 200000; dummy >0; dummy--);

}

void init_status_led(void)
/*
 * Input		: -
 * Output	: -
 * Function	: -
 */
{
  INT8S dummy;
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output.
  GPIO_PORTF_DIR_R |= PF1;

  // Enable the GPIO pins for digital function.
  GPIO_PORTF_DEN_R |= PF1;

  // Set pin high
  GPIO_PORTF_DATA_R |= PF1;
}

void systick_init(void)
/*
 * Input		: -
 * Output	: -
 * Function	: -
 */
{
	// Systick control and status register, NVIC_ST_CTRL_R
	// Systick reload register, NVIC_ST_RELOAD_R
	// Systick current value register, NVIC_ST_CURRENT_R
	// SYSTICK Calibration Value Register, NVIC_ST_CAL_R

	// Disable systick timer
	NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);
	// Set current systick counter to reload value
	NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;
	// Set Reload value, Systick reload register
	NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;
	// NVIC systick setup, vector number 15
	// Clear pending systick interrupt request
	NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;
	// Set systick priority to 0x10, first clear then set.
	NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
	NVIC_SYS_PRI3_R |= (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY<<NVIC_SYS_PRI3_TICK_S));
	// Select systick clock source, Use core clock
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
	// Enable systick interrupt
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
	// Enable and start timer
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}

void SysTick_Handler(void)
/*
 * Input		: -
 * Output	: -
 * Function	: See module description
 */
{
  // Hardware clears systick int reguest
  ticks++;
}

void enable_global_int()
/*
 * Input		: -
 * Output	: -
 * Function	: Enable global interrupts
 */
{
	__asm("cpsie i");
}

void disable_global_int()
/*
 * Input		: -
 * Output	: -
 * Function	: Disable global interrupts
 */
{
  __asm("cpsid i");
}

/****************************** End Of Module *******************************/
