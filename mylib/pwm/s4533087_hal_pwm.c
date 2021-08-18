/**
  *****************************************************************************
  * @file    s4533087_hal_pwm.c
  * @author  Alexander FitzGerald - s4533087
  * @date    11032020
  * @brief   PWM HAL driver
  *****************************************************************************
  * EXTERNAL FUNCTIONS 
  *****************************************************************************
  * s4533087_hal_pwm_init()
  *****************************************************************************
  */
 
/* Includes ---------------------------------------------------------------- */
#include "board.h"
#include "processor_hal.h"
#include "debug_printf.h"
#include "s4533087_hal_pwm.h"

/* Private typedef --------------------------------------------------------- */
/* Private define ---------------------------------------------------------- */
/* Private macro ----------------------------------------------------------- */
/* Private variables ------------------------------------------------------- */
TIM_HandleTypeDef S4533087_HAL_PWM_TIMER_HANDLER;
/* Private function prototypes --------------------------------------------- */

/**
  * @brief      Initialise and start PWM output(s)
  * @params     None
  * @retval     None
  */
void s4533087_hal_pwm_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OC_InitTypeDef PWMConfig;

    uint16_t PrescalerValue = 0;

    S4533087_HAL_PWM_PINCLK;
    __BRD_D6_GPIO_CLK();

    // Configure GPIO pin for use with PWM
    GPIO_InitStructure.Pin          = S4533087_HAL_PWM_PIN;
    GPIO_InitStructure.Mode         = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull         = GPIO_NOPULL;
    GPIO_InitStructure.Speed        = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate    = S4533087_HAL_PWM_PINGPIOAF;
    HAL_GPIO_Init(S4533087_HAL_PWM_PINGPIOPORT, &GPIO_InitStructure);

    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 
            S4533087_HAL_PWM_CLOCKFREQ) - 1;

    // Configure PWM timer
    S4533087_HAL_PWM_TIMER_HANDLER.Instance = S4533087_HAL_PWM_TIMER;
    S4533087_HAL_PWM_TIMER_HANDLER.Init.Period = S4533087_HAL_PWM_PERIOD;
    S4533087_HAL_PWM_TIMER_HANDLER.Init.Prescaler = PrescalerValue;
    S4533087_HAL_PWM_TIMER_HANDLER.Init.ClockDivision = 0;
    S4533087_HAL_PWM_TIMER_HANDLER.Init.RepetitionCounter = 0;
    S4533087_HAL_PWM_TIMER_HANDLER.Init.CounterMode = TIM_COUNTERMODE_UP;

    PWMConfig.OCMode        = TIM_OCMODE_PWM1;
    PWMConfig.Pulse         = S4533087_HAL_PWM_PULSEPERIOD;
    PWMConfig.OCPolarity    = TIM_OCPOLARITY_HIGH;
    PWMConfig.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
    PWMConfig.OCFastMode    = TIM_OCFAST_DISABLE;
    PWMConfig.OCIdleState   = TIM_OCIDLESTATE_RESET;
    PWMConfig.OCNIdleState  = TIM_OCNIDLESTATE_RESET;

    // Initialse and start
    HAL_TIM_PWM_Init(&S4533087_HAL_PWM_TIMER_HANDLER);
    HAL_TIM_PWM_ConfigChannel(&S4533087_HAL_PWM_TIMER_HANDLER, &PWMConfig, 
            S4533087_HAL_PWM_CHANNEL);

    HAL_TIM_PWM_Start(&S4533087_HAL_PWM_TIMER_HANDLER, 
            S4533087_HAL_PWM_CHANNEL);

    __HAL_TIM_GET_COMPARE(&S4533087_HAL_PWM_TIMER_HANDLER, 
            S4533087_HAL_PWM_CHANNEL);

}

/**
  * @brief      Returns the PWM duty cycle
  * @params     None
  * @retval     PWM duty cycle
  */
uint32_t s4533087_hal_pwm_dc_get(void) {
    
    return (uint32_t)__HAL_TIM_GET_COMPARE(&S4533087_HAL_PWM_TIMER_HANDLER, 
            S4533087_HAL_PWM_CHANNEL);
}

/**
  * @brief      Sets the PWM duty cycle
  * @params     PWM duty cycle
  * @retval     None
  */
void s4533087_hal_pwm_dc_set(uint32_t value) {

    __HAL_TIM_SET_COMPARE(&S4533087_HAL_PWM_TIMER_HANDLER, 
            S4533087_HAL_PWM_CHANNEL, value);
}
