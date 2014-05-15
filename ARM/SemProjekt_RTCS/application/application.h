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

/***************************** Include files *******************************/
/*****************************    Defines    *******************************/
extern QueueHandle_t QueuePositionPan,
extern QueueHandle_t QueuePositionTilt;
extern QueueHandle_t QueuePWMOutTilt;
extern QueueHandle_t QueuePWMOutPan;
extern QueueHandle_t QueueGoToPositionTilt;
extern QueueHandle_t QueueGoToPositionPan;
extern QueueHandle_t QueueUARTTX;
extern QueueHandle_t QueueUARTRX;
extern QueueHandle_t QueueSPITX;
extern QueueHandle_t QueueSPIRX;
extern QueueHandle_t QueuePanSpeed;
extern QueueHandle_t QueueTiltSpeed;
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
/****************************** End Of Module *******************************/
