/**
 ******************************************************************************
 * @file    timer_interrupt/main.c
 * @author  MDS, NPG
 * @date    02022015, 06122017
 * @brief   Enable Timer 2 update compare interrupt. Use the interrupt to flash
 * 			an LED every second.
 * 			See Section 18 (TIM2), P576 of the STM32F4xx Reference Manual.
 ******************************************************************************
 *
 */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef TIM_Init;
int count_interrupt;
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
void TIM2_IRQHandler(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program - Timer 2 compare update compare interrupt
 * @param  None
 * @retval None
 */
int main(void)
{
	BRD_init(); // Initialise Board
	Hardware_init(); // Initialise hardware peripherals

	/* Infinite loop */
	while (1)
	{
	}
}

/**
 * @brief  Initialise Hardware
 * @param  None
 * @retval None
 */
void Hardware_init(void) {

	unsigned short PrescalerValue;

	BRD_LEDInit();	//Initialise LEDs

	/* Turn off LEDs */
	BRD_LEDRedOff();
	BRD_LEDGreenOff();
	BRD_LEDBlueOff();

	// Timer 2 clock enable
	__TIM2_CLK_ENABLE();

	// Compute the prescaler value
	// Set the clock prescaler to 50kHz
	// SystemCoreClock is the system clock frequency
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 50000) - 1;

	/* TIM Base configuration */
	TIM_Init.Instance = TIM2;				//Enable Timer 2
  	TIM_Init.Init.Period = 50000/1000;			//Set period count to be 1ms, so timer interrupt occurs every 1ms.
  	TIM_Init.Init.Prescaler = PrescalerValue;	//Set prescaler value
  	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0;	// Set reload Value
  	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Initialise Timer */
	HAL_TIM_Base_Init(&TIM_Init);

	/* Set priority of Timer 2 update Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	/* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);		//Set Main priority to 10 and sub-priority to 0.

	// Enable the Timer 2 interrupt
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// Start Timer 2 base unit in interrupt mode
	HAL_TIM_Base_Start_IT(&TIM_Init);
}

/**
 * @brief Period elapsed callback in non blocking mode
 * @param htim: Pointer to a TIM_HandleTypeDef that contains the configuration information for the TIM module.
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		if (count_interrupt > 1000) {
			/* Toggle all 3 board LEDs */
			BRD_LEDToggle(BRD_GREEN_LEDMASK|BRD_BLUE_LEDMASK|BRD_RED_LEDMASK);
			count_interrupt = 0;
		}

		count_interrupt++;

	}
}

//Override default mapping of this handler to Default_Handler
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_Init);
}
