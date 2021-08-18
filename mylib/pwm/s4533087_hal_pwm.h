/**
  *****************************************************************************
  * @file   s4533087_hal_pwm.h
  * @author Alexander FitzGerald - s4533087
  * @date   11032020
  * @brief  PWM HAL driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_pwm_init()
  *****************************************************************************
  */

#ifndef S4533087_HAL_PWM_H
#define S4533087_HAL_PWM_H

#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#else

#define S4533087_HAL_PWM_CLOCKFREQ          100
#define S4533087_HAL_PWM_PULSEPERIOD        100
#define S4533087_HAL_PWM_PERIOD             100
#define S4533087_HAL_PWM_CHANNEL            0
#define S4533087_HAL_PWM_TIMER              0
#define S4533087_HAL_PWM_TIMER_CLK()        0
#define S4533087_HAL_PWM_TIMER_HANDLER      0
#define S4533087_HAL_PWM_PIN                BRD_D0_PIN
#define S4533087_HAL_PWM_PINGPIOPORT        BRD_D0_GPIO_PORT
#define S4533087_HAL_PWM_PINGPIOAF          0
#define S4533087_HAL_PWM_PINCLK             0
#define S4533087_HAL_PWM_PINCLKSPEED        0

#endif

// Macros
#define S4533087_HAL_PWM_DC_GET()       s453087_hal_pwm_dc_get()
#define S4533087_HAL_PWM_DC_SET(value)  s4533087_hal_pwm_dc_set(value)



// Function prototypes
void s4533087_hal_pwm_init(void);
uint32_t s4533087_hal_pwm_dc_get(void);
void s4533087_hal_pwm_dc_set(uint32_t);

#endif
