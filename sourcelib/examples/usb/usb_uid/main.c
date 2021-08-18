/**
  ******************************************************************************
  * @file    ex1_led/main.c 
  * @author  MDS
  * @date    1932016
  * @brief   NP2 onboard blue LED flashing example.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h" 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CURSOR_STEP     5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef myUSBD_Device;
//extern 
PCD_HandleTypeDef hpcd;
uint8_t HID_Buffer[4];

#define LEFT_BUTTON	0x04
#define MIDDLE_BUTTON	0x02
#define RIGHT_BUTTON	0x01

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
static void GetPointerData(uint8_t *pbuf);

/**
  * @brief  Main program - flashes onboard LED
  * @param  None
  * @retval None
  */
void main(void)  {

	BRD_init();			//Initalise Board
	Hardware_init();	//Initalise hardware modules
	int xi, yi;
	int x_mode=1;
	int y_mode=1;

	xi = 0;
	yi = 0;  	

	debug_printf("Starting\n\r");

	/* Init Device Library */
  	USBD_Init(&myUSBD_Device, &HID_Desc, 0);
  
  	/* Add Supported Class */
  	USBD_RegisterClass(&myUSBD_Device, USBD_HID_CLASS);
  
  	/* Start Device Process */
  	USBD_Start(&myUSBD_Device);

	/* Main processing loop */
    while (1) {

		//debug_printf("LED Toggle %d time: %d\n\r", i, HAL_GetTick());	//Print debug message with system time (us)
		
		BRD_LEDToggle();	//Toggle LED on/off
		HAL_Delay(10);		//Delay for 1s (1000ms)

	//	GetPointerData(HID_Buffer);

		HID_Buffer[0] = LEFT_BUTTON;	//press left button    

		if (x_mode == 1) {
			HID_Buffer[1] = CURSOR_STEP;
			xi++;
		} else {
			HID_Buffer[1] = -CURSOR_STEP;
			xi--;
		}


		if (y_mode == 1) {
			HID_Buffer[2] = -CURSOR_STEP;
			yi++;
		} else {
			HID_Buffer[2] = CURSOR_STEP;
			yi--;
		}

		if ((xi > 600) && (x_mode == 1)) {
			x_mode = 0;
		}

		if ((xi < 0) && (x_mode == 0)) {
			x_mode = 1;
		}


		if ((yi > 100) && (y_mode == 1)) {
			y_mode = 0;
		}

		if ((yi < 0) && (y_mode == 0)) {
			y_mode = 1;
		}

		USBD_HID_SendReport(&myUSBD_Device, HID_Buffer, 4);
	}
}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOn();		//Turn off Blue LED
}

/**
  * @brief  Gets Pointer Data.
  * @param  pbuf: Pointer to report
  * @retval None
  */
static void GetPointerData(uint8_t *pbuf)
{
  static int8_t cnt = 0;
  int8_t  x = 0, y = 0 ;
  
  if(cnt++ > 0)
  {
    x = CURSOR_STEP;
  }
  else
  {
    x = -CURSOR_STEP;
  }
  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
}

