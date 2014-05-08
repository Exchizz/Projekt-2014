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
#include "inc/binary.h"
/*****************************    Defines    *******************************/
#define SSI_CLK	2
#define SSI_SS 	3
#define SSI_RX	4
#define SSI_TX 	5
#define SSE_BIT	1

#define SPH 	0
#define SPO		0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void spi_enable( void )
/*
 * Function	:	Enables SPI
 */
{
	SET_BIT_HIGH(SSI0_CR1_R, SSE_BIT);
}

void spi_disable( void )
/*
 * Function :	Disables SPI
 */
{
	SET_BIT_LOW(SSI0_CR1_R, SSE_BIT);
}

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
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R, SSI_CLK);
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R, SSI_SS);
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R, SSI_RX);
	SET_BIT_HIGH(GPIO_PORTA_AFSEL_R, SSI_TX);

	// Configure PAD: 	2mA drive: set pins in GPIO_PORTA_DR2R_R
	SET_BIT_HIGH(GPIO_PORTA_DR2R_R, SSI_CLK);
	SET_BIT_HIGH(GPIO_PORTA_DR2R_R, SSI_SS);
	SET_BIT_HIGH(GPIO_PORTA_DR2R_R, SSI_RX);
	SET_BIT_HIGH(GPIO_PORTA_DR2R_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_ODR_R
	SET_BIT_LOW(GPIO_PORTA_ODR_R, SSI_CLK);
	SET_BIT_LOW(GPIO_PORTA_ODR_R, SSI_SS);
	SET_BIT_LOW(GPIO_PORTA_ODR_R, SSI_RX);
	SET_BIT_LOW(GPIO_PORTA_ODR_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_PUR_R
	SET_BIT_LOW(GPIO_PORTA_PUR_R, SSI_CLK);
	SET_BIT_LOW(GPIO_PORTA_PUR_R, SSI_SS);
	SET_BIT_LOW(GPIO_PORTA_PUR_R, SSI_RX);
	SET_BIT_LOW(GPIO_PORTA_PUR_R, SSI_TX);

	//	Clear pins in GPIO_PORTA_PDR_R
	SET_BIT_LOW(GPIO_PORTA_PDR_R, SSI_CLK);
	SET_BIT_LOW(GPIO_PORTA_PDR_R, SSI_SS);
	SET_BIT_LOW(GPIO_PORTA_PDR_R, SSI_RX);
	SET_BIT_LOW(GPIO_PORTA_PDR_R, SSI_TX);

	//	Set pins in GPIO_PORTA_DEN_R
	SET_BIT_HIGH(GPIO_PORTA_DEN_R, SSI_CLK);
	SET_BIT_HIGH(GPIO_PORTA_DEN_R, SSI_SS);
	SET_BIT_HIGH(GPIO_PORTA_DEN_R, SSI_RX);
	SET_BIT_HIGH(GPIO_PORTA_DEN_R, SSI_TX);

	SET_BIT_HIGH(GPIO_PORTA_DIR_R, SSI_CLK);
	SET_BIT_HIGH(GPIO_PORTA_DIR_R, SSI_SS);
	SET_BIT_HIGH(GPIO_PORTA_DIR_R, SSI_RX);
	SET_BIT_HIGH(GPIO_PORTA_DIR_R, SSI_TX);


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
	//SSI0_CR0_R = 0b00001001000001011;
	SSI0_CR0_R = (0b00001001000001011 | (( SPH << 7) | (SPO << 6)));
	//init_spi_interrupt(); //??????

	// 5 Enable SSI
			// Set SSI bit in SSICR1
	spi_enable();
}

void init_SPI_RX_task(){
	UARTprintf("Starting SPI rx\r\n");
	_start(SPI_RX_TASK, MILLI_SEC(0));
}
void init_SPI_TX_task(){
	UARTprintf("Starting SPI tx\r\n");
	_start(SPI_TX_TASK, MILLI_SEC(0));
}


BOOLEAN SPIDataReady(INT8U direction)
{
	BOOLEAN retVal = FALSE;

	if(direction)
		retVal = SSI0_SR_R & SSI_SR_TNF;
	else
		retVal = SSI0_SR_R & SSI_SR_RNE;

	return retVal;
}

INT16U SPICharGet(){
	return SSI0_DR_R;
}


void SPI_RX_task(){
	INT16U receivedChar;
	if(SPIDataReady(RX) && QueueEmpty(QueueSPIRX)){
		receivedChar = SPICharGet();
		QueueSend(QueueSPIRX, &receivedChar);
	}
}


void SPI_TX_task(){
	//wait for queue to get empty
	while(!SPIDataReady(TX));
	//Send next char
	INT16U dataFromQueue;
	if(QueueReceive(QueueSPITX, &dataFromQueue)) {
		SSI0_DR_R = dataFromQueue;
	}
}

/*
BOOLEAN GetTicks(){
	INT16U dataToSPI = 0;
	QueueSend(QueueSPITX, &dataToSPI);
	while
}


INT16U GetTicksPan(INT16U* msg){
  do {
	  GetTicks();
  } while((msgFromSPI & 0b110000000000) == 0b110000000000);


return msgFromSPI;
}


INT16U GetTicksTilt(INT16U* msg){
  do {
	  GetTicks();
  } while((msgFromSPI & 0b110000000000) == 0b100000000000);
  return msgFromSPI;
}
*/
/****************************** End Of Module *******************************/
