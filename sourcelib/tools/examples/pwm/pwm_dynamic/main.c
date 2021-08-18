/**
	******************************************************************************
	* @file		ex6_pwm_dynamic/src/main.c
	* @author	MDS
	* @date		08032017
	* @brief	 Produces a dynamic PWM  Waveform on pin D3.
	*			 See Section 18 (TIM3), P576 of the STM32F4xx Reference Manual.
	*
	*			NOTE: Refer to lineS 3163 and 4628 of the stm32f4xx_hal_tim.c, and
    *			lines 960 and 974 of stm32f4xx_hal_tim.h files. Refer to pages
    *			102-103 of the STM32F4-Technical-Training.pdf and page 590 of the
    *			STM32F4XX_reference.pdf. It's pretty good.
	******************************************************************************
	*
	*/

#include "board.h"
#include "processor_hal.h"

#define PWM_CLOCKFREQ		50000
#define PWM_PULSEPERIOD		(PWM_CLOCKFREQ/100)
#define PWM_PERIOD			2*PWM_CLOCKFREQ/10
#define PWM_CHANNEL			TIM_CHANNEL_3
#define PWM_PIN				BRD_D3_PIN
#define PWM_TIMER			TIM1
#define PWM_GPIO_AF			GPIO_AF1_TIM1
#define PWM_PIN_CLK()		__TIM1_CLK_ENABLE()
#define PWM_TIMER_HANDLER	TIM_Init
#define PWM_DC_GET() 		__HAL_TIM_GET_COMPARE(&PWM_TIMER_HANDLER, PWM_CHANNEL)
#define PWM_DC_SET(value) 	__HAL_TIM_SET_COMPARE(&PWM_TIMER_HANDLER, PWM_CHANNEL, value)

TIM_HandleTypeDef TIM_Init;
void Hardware_init();

int main(void) {

	uint32_t current_val = 0;
	uint32_t next_val = 0;
	uint8_t mode = 0;

	BRD_init();
	Hardware_init();

	while (1) {

		BRD_LEDRedToggle();
		HAL_Delay(40);

		current_val = PWM_DC_GET();

		if (mode) {
			next_val = (uint32_t)(current_val*1.1);
		} else {
			next_val = (uint32_t)(current_val*0.9);
		}

		PWM_DC_SET(next_val);

		if ((next_val > PWM_PULSEPERIOD*20) && (mode == 1)) {
			mode = 0;
		}

		if ((next_val < PWM_PULSEPERIOD) && (mode == 0)) {
			mode = 1;
		}
	}

	return 1;
}

void Hardware_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OC_InitTypeDef PWMConfig;

	uint16_t PrescalerValue = 0;

	BRD_LEDInit();
	BRD_LEDRedOff();

	PWM_PIN_CLK();
	__BRD_D3_GPIO_CLK();

	GPIO_InitStructure.Pin = PWM_PIN;
	GPIO_InitStructure.Mode =GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate = PWM_GPIO_AF;
	HAL_GPIO_Init(BRD_D3_GPIO_PORT, &GPIO_InitStructure);

	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / PWM_CLOCKFREQ) - 1;

	TIM_Init.Instance = PWM_TIMER;
	TIM_Init.Init.Period = PWM_PERIOD;
	TIM_Init.Init.Prescaler = PrescalerValue;
	TIM_Init.Init.ClockDivision = 0;
	TIM_Init.Init.RepetitionCounter = 0;
	TIM_Init.Init.CounterMode = TIM_COUNTERMODE_UP;

	PWMConfig.OCMode = TIM_OCMODE_PWM1;
	PWMConfig.Pulse = PWM_PULSEPERIOD;
	PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWMConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;
	PWMConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
	PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	HAL_TIM_PWM_Init(&TIM_Init);
	HAL_TIM_PWM_ConfigChannel(&TIM_Init, &PWMConfig, PWM_CHANNEL);

	HAL_TIM_PWM_Start(&TIM_Init, PWM_CHANNEL);

}

