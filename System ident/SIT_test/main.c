#include "lm3s6965.h"
#include "emp_type.h"

#define BAUD_RATE   						115200	// Baud rate [bits/s]
// Calculate baud rate divisor
#define B_DIV_TEMP	 	((15000000 /BAUD_RATE)<<2)
#define B_DIV_INT		B_DIV_TEMP>>6
// Calculate fraction part
#define B_DIV_FRACT 	(0x0003F & B_DIV_TEMP)

#define FALSE 	0
#define TRUE 	1

void UART_init(void);
BOOLEAN UART_put_char(INT8U character);

void PWM_init(void);

void encoder_init(void);
void get_encoder_direction(void);

void sample_time_init();

INT16U ticks = 32767;
INT8U direction = 0;

int main(void)
{
	PWM_init();

	INT32U i;
	for (i = 0; i < 9000000; i++);

	UART_init();
	encoder_init();
	sample_time_init();

	PWM_ENABLE_R |= PWM_ENABLE_PWM2EN; // Enable PWM output

	while(1)
    {
		get_encoder_direction();

		UART_put_char(ticks >>8);
		UART_put_char(ticks);
		UART_put_char(direction);
		GPIO_PORTB_DATA_R ^= 0x10;
    }
}

void get_encoder_direction(void)
{
	INT8U PB2, PB3, YY;
	INT32U i;
	static INT8U lastPB2 = 0, lastPB3 = 0;

	PB2 = GPIO_PORTB_DATA_R & 0x4;
	PB3 = GPIO_PORTB_DATA_R & 0x8;

	PB2 = (PB2 >> 1);
	PB3 = (PB3 >> 3);

	if (!(PB2 == lastPB2 && PB3 == lastPB3))
	{
		YY = (PB2 | PB3) ^ (lastPB2 | lastPB3);

		if ((PB2 | PB3) == 3 || (PB2 | PB3) == 0)
		{
			if (YY == 1)
			{
				ticks++;// CW
			}
			else if(YY == 2)
			{
				ticks--;// CCW
			}
		}
		else
		{
			if (YY == 2)
			{
				ticks++;// CW
			}
			else if(YY == 1)
			{
				ticks--;// CCW
			}
		}

		lastPB2 = PB2;
		lastPB3 = PB3;
	}

	if (ticks == 360+32767)
	{
		PWM_ENABLE_R &= ~PWM_ENABLE_PWM2EN; // Enable PWM output
		PWM_ENABLE_R |= PWM_ENABLE_PWM3EN; // Enable PWM output
		direction = 1;
	}

	if (ticks == 32767)
	{
		PWM_ENABLE_R &= ~PWM_ENABLE_PWM3EN; // Enable PWM output
		PWM_ENABLE_R |= PWM_ENABLE_PWM2EN; // Enable PWM output
		direction = 0;
	}
}

void sample_time_init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

	//Set direction as input
	GPIO_PORTB_DIR_R |= 0x10;

	//enable digital function
	GPIO_PORTB_DEN_R |= 0x10;
}

void encoder_init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

	//Set direction as input
	GPIO_PORTB_DIR_R &= ~0xC;

	//enable digital function
	GPIO_PORTB_DEN_R |= 0xC;
}

void PWM_init(void)
{

	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM; // Enable the PWM clock

	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // Enable the clock to PORTB

	GPIO_PORTB_AFSEL_R |= 0x3; // Enable the PB0 alternate function
	GPIO_PORTB_DEN_R |= 0x3;

	SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; //Enable PWM divide
	SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;
	SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_2;

	//Magic!
	PWM_1_CTL_R &= ~PWM_X_CTL_ENABLE;
	PWM_1_GENA_R |= 0x8C;
	PWM_1_GENB_R |= 0x80C;

	PWM_1_LOAD_R = 299;

	PWM_1_CMPA_R = 299/2;
	PWM_1_CMPB_R = 299/2;

	PWM_1_CTL_R |= PWM_X_CTL_ENABLE; // Start the timers

	PWM_ENABLE_R &= ~(PWM_ENABLE_PWM2EN | PWM_ENABLE_PWM3EN);
}

BOOLEAN UART_put_char(INT8U character)
/*****************************************************************************
*   Function : See module specification (.h-file).
******************************************************************************/
{
	BOOLEAN result = FALSE;

	while((UART0_FR_R & UART_FR_TXFF));

	UART0_DR_R = character;
	result = TRUE;


	return result;
}

void UART_init(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
******************************************************************************/
{
	// Enable UART0 peripherial
	SYSCTL_RCGC1_R  |= SYSCTL_RCGC1_UART0;

	//Enable the pins used by the UART
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;

	// PORTA [0:1] to alternative function
	GPIO_PORTA_AFSEL_R |= 0x03;

	// Enable digital function
	GPIO_PORTA_DEN_R |= 0x03;

	// Disable UART
	UART0_CTL_R &= ~UART_CTL_UARTEN;

	// Clear UART pending interrupt.
	UART0_ICR_R |= 0x7F0;

	// Baud rate setup.
	UART0_IBRD_R = B_DIV_INT;
	UART0_FBRD_R = B_DIV_FRACT;

	// UART setup
	// Clear Line control register
	// UART0_LCRH_R = 0;
	// 8 bit data, 1 stop bit, no parity.
	// Enable FIFO
	UART0_LCRH_R |= UART_LCRH_WLEN_8 | UART_LCRH_FEN;

	// Uart0 enable transmitter
	UART0_CTL_R |= UART_CTL_TXE | UART_CTL_RXE;

	// Enable UART0
	UART0_CTL_R |= UART_CTL_UARTEN;
}
