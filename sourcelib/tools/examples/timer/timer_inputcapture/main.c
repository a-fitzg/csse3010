/**
  ******************************************************************************
  * @file    timer_inputcapture/main.c
  * @author  MDS
  * @date    02032018
  * @brief   Enable the Timer Input capture on pin A6.
  *			 See Section 18 (TIM3), P592 of the STM32F4xx Reference Manual.
  ******************************************************************************
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef TIM_Init;

/* Private function prototypes -----------------------------------------------*/
void Hardware_init();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void) {

	BRD_init();	//Initalise Board
	Hardware_init();	//Initalise hardware modules

  	while (1) {
	}
}

/**
  * @brief  Configure the hardware,
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_IC_InitTypeDef  TIM_ICInitStructure;
	uint16_t PrescalerValue = 0;

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDRedOff();		//Turn off Blue LED

  	/* Timer 3 clock enable */
  	__TIM3_CLK_ENABLE();

  	/* Enable the D0 Clock */
  	__BRD_A6_GPIO_CLK();

  	/* Configure the D0 pin with TIM3 input capture */
	GPIO_InitStructure.Pin = BRD_A6_PIN;				//Pin
  	GPIO_InitStructure.Mode =GPIO_MODE_AF_PP; 		//Set mode to be output alternate
  	GPIO_InitStructure.Pull = GPIO_NOPULL;			//Enable Pull up, down or no pull resister
  	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;			//Pin latency
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;	//Set alternate function to be timer 2
  	HAL_GPIO_Init(BRD_A6_GPIO_PORT, &GPIO_InitStructure);	//Initialise Pin

	/* Compute the prescaler value. Set for 50Khz clock */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 50000) - 1;

	/* Configure Timer 3 settings */
	TIM_Init.Instance = TIM3;					//Enable Timer 3
  	TIM_Init.Init.Period = 2*50000/10;			//Set for 100ms (10Hz) period
  	TIM_Init.Init.Prescaler = PrescalerValue;	//Set presale value
  	TIM_Init.Init.ClockDivision = 0;			//Set clock division
	TIM_Init.Init.RepetitionCounter = 0; 		// Set Reload Value
  	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;	//Set timer to count up.

	/* Configure TIM3 Input capture */
  	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;			//Set to trigger on rising edge
  	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
  	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStructure.ICFilter = 0;

	/* Set priority of Timer 3 Interrupt [0 (HIGH priority) to 15(LOW priority)] */
	HAL_NVIC_SetPriority(TIM3_IRQn, 10, 0);	//Set Main priority ot 10 and sub-priority ot 0.

	/* Enable Timer 3 interrupt and interrupt vector */
	NVIC_EnableIRQ(TIM3_IRQn);

	/* Enable input capture for Timer 3, channel 4 */
	HAL_TIM_IC_Init(&TIM_Init);
	HAL_TIM_IC_ConfigChannel(&TIM_Init, &TIM_ICInitStructure, TIM_CHANNEL_4);

	/* Start Input Capture */
	HAL_TIM_IC_Start_IT(&TIM_Init, TIM_CHANNEL_4);

}


/**
  * @brief  Timer 1 Input Capture Interrupt handler
  * @param  None.
  * @retval None
  */
void HAL_TIM3_IRQHandler (TIM_HandleTypeDef *htim) {

	unsigned int input_capture_value;

	/* Clear Input Capture Flag */
	__HAL_TIM_CLEAR_IT(&TIM_Init, TIM_IT_TRIGGER);

	/* Toggle LED */
	BRD_LEDRedToggle();

  	/* Read and display the Input Capture value of Timer 3, channel 4 */
  	input_capture_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

  	debug_printf("Input capture: %d\n\r", input_capture_value);

}

/**
  * @brief  Timer 1 Input Capture Interrupt handler
  * Override default mapping of this handler to Default_Handler
  * @param  None.
  * @retval None
  */
void TIM3_IRQHandler(void) {
	HAL_TIM3_IRQHandler(&TIM_Init);
	BRD_LEDRedToggle();
}

