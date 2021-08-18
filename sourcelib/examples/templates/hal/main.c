/**
  ******************************************************************************
  * @file    ex_HAL/main.c
  * @author  MDS
  * @date    27112019
  * @brief   HAL template.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);

/**
  * @brief  Main program - flashes onboard LEDs
  * @param  None
  * @retval None
  */
void main(void)  {

	BRD_init();			//Initalise Board
	Hardware_init();	//Initalise hardware modules
	int i;

	i = 0;

	/* Main processing loop */
    while (1) {

		debug_printf("LED Toggle %d time: %d\n\r", i, HAL_GetTick());	//Print debug message with system time (us)

		i++;

		/* Toggle all LEDs */
		BRD_LEDRedToggle();
		BRD_LEDGreenToggle();
		BRD_LEDBlueToggle();

		HAL_Delay(1000);		//Delay for 1s (1000ms)

	}
}

/**
  * @brief  Initialise Hardware
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	BRD_LEDInit();		//Initialise LEDS

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();
}
