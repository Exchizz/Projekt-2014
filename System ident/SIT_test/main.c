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

INT16U ticks = 0;
INT8U direction = 0, n = 0;
INT8U array[1000] = {115, 204, 21, 99, 251, 53, 49, 10, 84, 213, 40, 165, 237, 92, 106,
		3, 94, 221, 21, 234, 206, 133, 192, 99, 73, 163, 18, 121, 248, 219,
		16, 155, 61, 228, 295, 83, 227, 266, 180, 216, 155, 156, 211, 156,
		179, 17, 148, 145, 50, 29, 194, 265, 232, 166, 48, 97, 204, 264, 283,
		16, 44, 132, 271, 47, 16, 12, 187, 151, 150, 158, 58, 150, 29, 157,
		266, 111, 246, 185, 256, 126, 137, 176, 127, 127, 113, 262, 274, 180,
		152, 45, 262, 181, 180, 132, 121, 62, 35, 175, 2, 267, 69, 246, 31,
		70, 11, 149, 135, 277, 57, 168, 178, 285, 252, 210, 164, 77, 228,
		228, 67, 249, 120, 251, 257, 275, 226, 235, 116, 104, 75, 249, 230,
		286, 190, 276, 236, 63, 173, 58, 262, 28, 113, 40, 138, 213, 201, 93,
		133, 18, 73, 81, 80, 282, 184, 210, 150, 126, 130, 22, 32, 104, 222,
		40, 253, 4, 153, 268, 248, 277, 267, 118, 144, 131, 82, 146, 12, 44,
		156, 195, 109, 294, 65, 257, 35, 123, 28, 93, 274, 100, 168, 106,
		235, 241, 227, 111, 78, 177, 258, 174, 219, 272, 204, 25, 245, 35,
		53, 147, 156, 95, 158, 47, 287, 65, 8, 98, 128, 254, 42, 80, 48, 18,
		165, 114, 41, 132, 276, 159, 87, 57, 90, 39, 137, 53, 161, 92, 2,
		290, 185, 246, 225, 196, 59, 111, 170, 294, 235, 156, 203, 98, 140,
		292, 98, 48, 134, 197, 292, 257, 177, 178, 123, 55, 129, 55, 27, 238,
		232, 258, 133, 195, 223, 66, 62, 68, 256, 64, 195, 30, 67, 99, 183,
		243, 273, 31, 169, 46, 101, 250, 47, 150, 11, 152, 24, 280, 81, 216,
		1, 35, 212, 183, 151, 11, 236, 145, 85, 25, 174, 147, 147, 217, 287,
		250, 51, 237, 202, 37, 223, 104, 215, 128, 172, 65, 247, 122, 117,
		127, 67, 202, 218, 172, 36, 241, 285, 34, 128, 142, 118, 254, 165,
		216, 241, 106, 256, 73, 228, 91, 248, 285, 209, 75, 111, 11, 139,
		239, 137, 84, 101, 192, 138, 0, 122, 15, 196, 23, 142, 176, 238, 104,
		46, 205, 110, 273, 157, 123, 284, 89, 213, 195, 102, 63, 298, 67,
		239, 274, 26, 209, 78, 248, 15, 170, 30, 43, 263, 252, 53, 123, 31,
		118, 21, 137, 3, 274, 282, 155, 47, 243, 78, 49, 245, 27, 220, 206,
		26, 147, 209, 174, 52, 117, 196, 90, 245, 210, 114, 254, 47, 185,
		173, 128, 10, 46, 50, 232, 221, 262, 106, 281, 76, 134, 252, 42, 235,
		119, 175, 11, 97, 235, 70, 18, 97, 44, 290, 132, 199, 106, 193, 273,
		16, 287, 87, 60, 129, 152, 86, 10, 61, 120, 131, 20, 162, 14, 226,
		55, 64, 202, 119, 148, 282, 280, 200, 18, 80, 290, 132, 205, 27, 67,
		61, 80, 60, 33, 270, 92, 45, 202, 122, 293, 274, 266, 203, 12, 256,
		257, 106, 222, 233, 146, 199, 26, 268, 20, 194, 93, 76, 126, 191,
		257, 207, 37, 103, 148, 137, 16, 151, 133, 248, 210, 257, 33, 248, 4,
		166, 246, 282, 244, 23, 268, 49, 177, 38, 206, 91, 197, 89, 69, 73,
		253, 29, 260, 178, 201, 201, 152, 80, 261, 104, 187, 158, 218, 92, 0,
		156, 6, 14, 266, 72, 167, 109, 6, 150, 37, 96, 294, 32, 2, 57, 222,
		213, 119, 177, 124, 137, 141, 83, 258, 38, 85, 69, 269, 24, 161, 99,
		241, 55, 269, 232, 74, 44, 226, 12, 181, 118, 293, 228, 78, 191, 225,
		46, 207, 239, 34, 85, 119, 197, 88, 182, 64, 136, 16, 112, 248, 229,
		226, 207, 58, 139, 266, 15, 237, 145, 15, 114, 263, 74, 142, 154,
		126, 176, 82, 184, 184, 203, 229, 202, 52, 191, 5, 216, 91, 48, 258,
		81, 202, 275, 171, 206, 243, 284, 109, 108, 261, 165, 25, 290, 292,
		49, 208, 58, 163, 17, 111, 245, 55, 1, 250, 62, 214, 249, 265, 240,
		298, 271, 118, 249, 222, 25, 4, 222, 196, 137, 214, 291, 118, 9, 73,
		107, 140, 165, 232, 125, 271, 267, 203, 160, 71, 291, 170, 27, 243,
		119, 232, 4, 111, 202, 34, 226, 151, 240, 59, 36, 154, 159, 75, 22,
		292, 175, 276, 249, 237, 55, 161, 57, 278, 126, 266, 278, 157, 96,
		275, 243, 203, 12, 103, 287, 63, 45, 274, 278, 21, 250, 284, 207,
		180, 14, 152, 146, 276, 224, 103, 177, 11, 288, 17, 263, 20, 42, 156,
		241, 91, 69, 82, 128, 243, 119, 97, 215, 212, 79, 136, 14, 124, 104,
		249, 65, 128, 14, 104, 29, 152, 158, 131, 47, 200, 65, 215, 298, 253,
		71, 50, 216, 39, 237, 40, 108, 209, 262, 153, 248, 267, 15, 181, 294,
		145, 88, 87, 43, 112, 247, 190, 215, 29, 107, 254, 32, 272, 206, 94,
		22, 152, 58, 125, 181, 197, 149, 192, 104, 275, 143, 46, 75, 204, 46,
		236, 27, 212, 180, 22, 241, 189, 246, 7, 44, 222, 110, 191, 89, 91,
		67, 172, 104, 77, 10, 148, 164, 195, 188, 117, 168, 131, 40, 82, 147,
		15, 91, 57, 298, 80, 119, 298, 159, 23, 66, 158, 173, 62, 270, 231,
		70, 139, 171, 92, 12, 146, 68, 53, 156, 158, 180, 201, 59, 199, 103,
		69, 102, 271, 88, 205, 217, 225, 145, 185, 252, 217, 65, 241, 23,
		230, 47, 88, 50, 180, 103, 100, 226, 168, 159, 121, 260, 121, 51,
		215, 225, 0, 131, 127, 89, 110, 5, 26, 212, 213, 164, 287, 252, 216,
		173, 184, 46, 26, 168, 193, 170, 181, 280, 11, 186, 245, 184, 155,
		58, 207, 219, 226, 143, 161, 110, 253, 31, 263, 75, 165, 130, 131,
		105, 102, 199, 62, 17, 138, 199, 162, 157, 19, 203, 283, 61, 257,
		224, 121, 76, 277, 256, 80, 94, 226, 199, 26, 213, 13, 65, 258, 288,
		166};
int main(void)
{
	PWM_init();

	INT32U i;
	for (i = 0; i < 9000000; i++);
	i = 0;
	UART_init();
	encoder_init();
	sample_time_init();

	PWM_ENABLE_R |= PWM_ENABLE_PWM2EN; // Enable PWM output

	while(1)
    {
		get_encoder_direction();

		UART_put_char(ticks >> 8);
		UART_put_char(ticks);
		UART_put_char(array[n]);
		GPIO_PORTB_DATA_R ^= 0x10;

		if(!(i%3500))
		{
			PWM_1_CMPA_R = array[n];
			if(++n == 1000){
				n = 0;
			}
		}

		if(i == 52500)
			i=0;
		//	PWM_1_CMPA_R = 298/2;
		i++;
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
/*
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
*/
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

	PWM_1_LOAD_R = 298;

	PWM_1_CMPA_R = 298;
	PWM_1_CMPB_R = 298;

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
