/**
  *****************************************************************************
  * @file   s4533087_hal_mylib_configure.h
  * @author Alexander FitzGerald - s4533087
  * @date   14032020
  * @brief  
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * 
  *****************************************************************************
  */

#ifndef S4533087_HAL_MYLIB_CONFIGURE_H
#define S4533087_HAL_MYLIB_CONFIGURE_H

// Macros from atimer
#define S4533087_HAL_ATIMER_PIN             BRD_D10_PIN
#define S4533087_HAL_ATIMER_PINPORT         BRD_D10_GPIO_PORT
#define S4533087_HAL_ATIMER_PINCLK          __BRD_D10_GPIO_CLK()
#define S4533087_HAL_ATIMER_PERIOD          1000
#define S4533087_HAL_ATIMER_CLKSPEED        50000

// Macros from pwm
#define S4533087_HAL_PWM_CLOCKFREQ          50000
#define S4533087_HAL_PWM_PULSEPERIOD        (S4533087_HAL_PWM_CLOCKFREQ / 10)
#define S4533087_HAL_PWM_PERIOD             (S4533087_HAL_PWM_CLOCKFREQ / 1)
#define S4533087_HAL_PWM_CHANNEL            TIM_CHANNEL_1
#define S4533087_HAL_PWM_TIMER              TIM1
#define S4533087_HAL_PWM_TIMER_CLK()        __TIM1_CLK_ENABLE()
#define S4533087_HAL_PWM_TIMER_HANDLER      TIM_InitPWM
#define S4533087_HAL_PWM_PIN                BRD_D6_PIN
#define S4533087_HAL_PWM_PINGPIOPORT        BRD_D6_GPIO_PORT
#define S4533087_HAL_PWM_PINGPIOAF          GPIO_AF1_TIM1
#define S4533087_HAL_PWM_PINCLK             __TIM1_CLK_ENABLE()
#define S4533087_HAL_PWM_PINCLKSPEED        GPIO_SPEED_FAST

// Function prototypes

#endif
