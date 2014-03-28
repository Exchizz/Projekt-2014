/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			SPI (Serial Peripheral Interface (bus))
 * Author(s):		K.Andersen
 * Module:			spi.c
 * Description:	Initiates and controls the SPI
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 050128			KA			Module Created
 * 140311			NL			C Coding Standards for EMP invoked
 *
 */


/***************************** Include files *******************************/
#include "spi.h"

/*****************************    Defines    *******************************/
#define SSI_CLK	2
#define SSI_SS 	3
#define SSI_RX		4
#define SSI_TX 	5

#define SSE_BIT	1


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void spi_enable( void )
/*
 * Function	:	Enables SPI
 */
{
	SET_BIT(SSI0_CR1_R, SSE_BIT);
}

void spi_disable( void )
/*
 * Function :	Disables SPI
 */
{
	CLEAR_BIT(SSI0_CR1_R, SSE_BIT);
}
/*
void init_spi_interrupt()
{
	// SEE PAGE 484 -
	// PAGE 82 VECTOR NUMBER
	// PAGE 110 SET ENABLE

	// 1 Write to SSIIM (enable SSI interrupt)
		// TXIM	[3]		Transmit FIFO Interrupt Mask
		// RXIM	[2]		Receive FIFO Interrupt Mask
		// RTIM	[1]		Receive FIFO Timeout Interrupt Mask
		// RORIM [0]		Receive FIFO Overrun Interrupt Mask
		// Set bit to unmask interrupt, clear bit to mask interrupt
	SET_BIT(SSI0_IM_R, 2);




	// NVIC SSI setup, vector number 23
		// Clear pending SSI interrupt request
	SET_BIT(NVIC_INT_CTRL_R,23);

		// Set SSI priority to 0x10, first clear then set.
	NVIC_PRI5_R &= ~(NVIC_PRI5_INT23_M);
	NVIC_PRI5_R |= (NVIC_PRI5_INT23_M & (0x10 << NVIC_PRI5_INT23_S));
		// enable NVIC interrupt
	NVIC_EN0_R |= (1 << 23);

}
*/
/*
void SSI0_IRQHandler()
{
	DEBUG_INFO("Interrupt",0);
	// Clear Pending interrupts
	//CLEAR_BIT(SSI0_ICR_R, );
}
*/
void spi_init()
/*
 *	Function : Initiates SPI
 */
{
	INT8U dummy;

	// Pin setup
	// Enable port A.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	dummy = SYSCTL_RCGC2_R;

	// HW control, Set pins in GPIO_PORTA_AFSEL_R
	SET_BIT(GPIO_PORTA_AFSEL_R, SSI_CLK);
	SET_BIT(GPIO_PORTA_AFSEL_R, SSI_SS);
	SET_BIT(GPIO_PORTA_AFSEL_R, SSI_RX);
	SET_BIT(GPIO_PORTA_AFSEL_R, SSI_TX);

	// Configure PAD: 	2mA drive: set pins in GPIO_PORTA_DR2R_R
	SET_BIT(GPIO_PORTA_DR2R_R, SSI_CLK);
	SET_BIT(GPIO_PORTA_DR2R_R, SSI_SS);
	SET_BIT(GPIO_PORTA_DR2R_R, SSI_RX);
	SET_BIT(GPIO_PORTA_DR2R_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_ODR_R
	CLEAR_BIT(GPIO_PORTA_ODR_R, SSI_CLK);
	CLEAR_BIT(GPIO_PORTA_ODR_R, SSI_SS);
	CLEAR_BIT(GPIO_PORTA_ODR_R, SSI_RX);
	CLEAR_BIT(GPIO_PORTA_ODR_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_PUR_R
	CLEAR_BIT(GPIO_PORTA_PUR_R, SSI_CLK);
	CLEAR_BIT(GPIO_PORTA_PUR_R, SSI_SS);
	CLEAR_BIT(GPIO_PORTA_PUR_R, SSI_RX);
	CLEAR_BIT(GPIO_PORTA_PUR_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_PDR_R
	CLEAR_BIT(GPIO_PORTA_PDR_R, SSI_CLK);
	CLEAR_BIT(GPIO_PORTA_PDR_R, SSI_SS);
	CLEAR_BIT(GPIO_PORTA_PDR_R, SSI_RX);
	CLEAR_BIT(GPIO_PORTA_PDR_R, SSI_TX);

	//	Set pins in GPIO_PORTA_DEN_R
	SET_BIT(GPIO_PORTA_DEN_R, SSI_CLK);
	SET_BIT(GPIO_PORTA_DEN_R, SSI_SS);
	SET_BIT(GPIO_PORTA_DEN_R, SSI_RX);
	SET_BIT(GPIO_PORTA_DEN_R, SSI_TX);

	SET_BIT(GPIO_PORTA_DIR_R, SSI_CLK);
	SET_BIT(GPIO_PORTA_DIR_R, SSI_SS);
	SET_BIT(GPIO_PORTA_DIR_R, SSI_RX);
	SET_BIT(GPIO_PORTA_DIR_R, SSI_TX);


	// SSI setup

	// SYSCTL_RCGC1_R 	== RCGC1
	// SSI0_CR1_R 			== SSICR1
	// SSI0_CPSR_R 		== SSICPSR
	// SSI0_CR0_R			== SSICR0

	// Enable SSI Clock
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;

	// 1 Disable SSI
		// Clear SSI bit in SSICR1
	spi_disable();

	// 2 Select master or slave
		// For master operations, set the SSICR1 register to 0x0000.0000
	SSI0_CR1_R = 0x00000000;

	// 3 Configure clock
		// For master mode the system clock must be at least two times faster than
		// the SSIclk.

		// SSIclk = FSysClk / (CPSDVSR * (1 + SCR))

		// 50*10^6/(4*(1+12))
		// SysClock * 10^6 / (CPSDVSR * (1 + SCR))
		// (50 * (10^6)) / (4 * (1 + 9)) = 1 250 000 bit/sec

		// SSICPSR = CPSDVSR = 0x4
	SSI0_CPSR_R = 0x4;

	// 4 Write to SSICR0:
		// SCR: [15:8]	SSI Serial Clock Rate
		// SPH: [7]		SSI Serial Clock Phase
		// SPO:	[6]		SSI Serial Clock Polarity
		// FRF:	[5:4] 	FrameFormat, 0x0 for Freescale (SPI)
		// DSS:	[3:0] 	Datasize
		// SSI0_CR0_R = 0b00000000 0 0 00 0000
	SSI0_CR0_R = 0b0000100111001111;

	//init_spi_interrupt(); //??????

	// 5 Enable SSI
			// Set SSI bit in SSICR1
	spi_enable();
}

BOOLEAN spiDataReady(INT8U direction)
{
	BOOLEAN retVal = 0;

	if(direction)
		retVal = SSI0_SR_R & SSI_SR_TNF;
	else
		retVal = SSI0_SR_R & SSI_SR_RNE;

	return retVal;
}

void spiTXTask()
{
	INT16U dataToSend;
	while(TRUE)
	{
		if(xQueueReceive(SPITXQueue, &dataToSend, 50)){
			if(spiDataReady(TX)){
				SSI0_DR_R = dataToSend;
				//xQueueSend(UARTTXQueue, &dataToSend,50);
			}
			//else
				//some lcd error code
		}
	}
}

void spiRXTask()
{
	INT16U receivedData;
	while(TRUE)
	{
		if(xSemaphoreTake(SPIRXSem,20)){
			if(spiDataReady(RX)){
				receivedData = SSI0_DR_R;
				xQueueSend(SPIRXQueue, &receivedData, 50);
			}
		}
	}
}

/****************************** End Of Module *******************************/
