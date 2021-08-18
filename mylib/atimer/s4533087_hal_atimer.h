/**
  *****************************************************************************
  * @file   s4533087_hal_atimer.h
  * @author Alexander FitzGerald - s4533087
  * @date   11032020
  * @brief  Alive timer HAL driver header
  *****************************************************************************
  * EXTERNAL FUNCTIONS
  *****************************************************************************
  * s4533087_hal_atimer_init()
  * s4533087_hal_atimer_timer_getms()
  * s4533087_hal_atimer_timer_reset()
  * s4533087_hal_atimer_timer_pause()
  * s4533087_hal_atimer_timer_resume()
  * s4533087_hal_atimer_timer_printticks();
  * s4533087_hal_atimer_timer_printms();
  * s4533087_hal_atimer_timer_addperiod();
  * s4533087_hal_atimer_timer_addfrequency();
  * s4533087_hal_atimer_clkspeed_set()
  * s4533087_hal_atimer_period_set()
  * s4533087_hal_atimer_init_pin()
  *****************************************************************************
  */

#ifndef S4533087_HAL_ATIMER_H
#define S4533087_HAL_ATIMER_H

// Includes
#ifdef S4533087_MYLIB_CONFIGURE_INC
#include "s4533087_mylib_configure.h"
#else

#define S4533087_HAL_ATIMER_PIN             BRD_D0_PIN
#define S4533087_HAL_ATIMER_PINPORT         BRD_D0_GPIO_PORT
#define S4533087_HAL_ATIMER_PINCLK          __BRD_D0_GPIO_CLK()
#define S4533087_HAL_ATIMER_PERIOD          1000
#define S4533087_HAL_ATIMER_CLKSPEED        50000

#endif

// Function prototypes
void s4533087_hal_atimer_init(void);
uint32_t s4533087_hal_atimer_timer_getms(void);
void s4533087_hal_atimer_timer_reset(void);
void s4533087_hal_atimer_timer_pause(void);
void s4533087_hal_atimer_timer_resume(void);
void s4533087_hal_atimer_timer_printticks(void);
void s4533087_hal_atimer_timer_printms(void);
void s4533087_hal_atimer_timer_addperiod(int);
void s4533087_hal_atimer_timer_addfrequency(int);
void s4533087_hal_atimer_clkspeed_set(uint32_t frequency);
void s4533087_hal_atimer_period_set(uint32_t period);
void s4533087_hal_atimer_init_pin(void);

#endif
