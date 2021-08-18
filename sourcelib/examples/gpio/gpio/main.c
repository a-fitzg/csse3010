/**
  ******************************************************************************
  * @file    ex_HAL/main.c
  * @author  MDS
  * @date    27112017
  * @brief   Nucleo429ZI onboard LED flashing example.
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
  * @brief  Main program - flashes on board LEDs
  * @param  None
  * @retval None
  */
int main(void)  {

	int read_value;
	int write_value = 0;

	BRD_init();			//Initialise board.
	Hardware_init();	//Initialise hardware modules

	/* Main processing loop */
    while (1) {

		/* Toggle D0 high or low */
		write_value = ~write_value;
		HAL_GPIO_WritePin(BRD_D0_GPIO_PORT, BRD_D0_PIN, write_value & 0x01);	//Write Digital 0 bit value

		/* Read D1 pin */
		read_value = HAL_GPIO_ReadPin(BRD_D1_GPIO_PORT, BRD_D1_PIN);

		debug_printf("D1 value %d\n\r", read_value);	//Display D1 value

    	BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);	//Toggle LED on/off
		HAL_Delay(1000);	//Delay for 1000ms.
  	}

    return 0;
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

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the D0 & D1 Clock */
  	__BRD_D0_GPIO_CLK();
	__BRD_D1_GPIO_CLK();

  	/* Configure the D0 pin as an output */
	GPIO_InitStructure.Pin = BRD_D0_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;		//Output Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(BRD_D0_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin

	/* Configure the D1 pin as an input */
	GPIO_InitStructure.Pin = BRD_D1_PIN;				//Pin
  	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;			//Input Mode
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
  	HAL_GPIO_Init(BRD_D1_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin
}
